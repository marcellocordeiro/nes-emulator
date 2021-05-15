#include "PPU.h"

#include <algorithm>

#include "Cartridge.h"
#include "Utility/FileManager.h"

using namespace nes::types::ppu;
using namespace nes::types::cartridge;

namespace nes {
auto PPU::get() -> PPU&
{
  static PPU instance;
  return instance;
}

void PPU::power_on()
{
  this->set_palette();

  ctrl.raw   = 0;
  mask.raw   = 0;
  status.raw = 0;
  oam_addr   = 0;
  addr_latch = false;

  vram_addr.raw = 0;
  temp_addr.raw = 0;
  fine_x        = 0;
  oam_addr      = 0;

  bus_latch      = 0;
  ppudata_buffer = 0;

  is_odd_frame = false;
}

void PPU::reset()
{
  ctrl.raw   = 0;
  mask.raw   = 0;
  addr_latch = false;

  vram_addr.raw = 0;
  temp_addr.raw = 0;
  fine_x        = 0;

  bus_latch      = 0;
  ppudata_buffer = 0;

  is_odd_frame = false;
}

auto PPU::get_back_buffer() const -> const uint32_t* { return back_buffer.data(); }

void PPU::set_palette()
{
  auto palette = Utility::FileManager::get().get_palette();

  if (palette.size() != 64 * 3) {
    throw std::invalid_argument("Invalid palette file");
  }

  for (size_t i = 0; i < 64; ++i) {
    uint8_t r = palette[(i * 3) + 0];
    uint8_t g = palette[(i * 3) + 1];
    uint8_t b = palette[(i * 3) + 2];

    full_nes_palette[0][i] = (r << 16) | (g << 8) | b;
  }

  // Generate full colour palette
  for (size_t i = 1; i < 8; ++i) {
    for (size_t j = 0; j < 64; ++j) {
      double red   = static_cast<uint8_t>(full_nes_palette[0][j] >> 16);
      double green = static_cast<uint8_t>(full_nes_palette[0][j] >> 8);
      double blue  = static_cast<uint8_t>(full_nes_palette[0][j] >> 0);

      constexpr double factor = 0.3;

      // Intensify red
      if (i & 0x01) {
        red *= 1 + factor;
        green *= 1 - factor;
        blue *= 1 - factor;
      }

      // Intensify green
      if (i & 0x02) {
        red *= 1 - factor;
        green *= 1 + factor;
        blue *= 1 - factor;
      }

      // Intensify blue
      if (i & 0x04) {
        red *= 1 - factor;
        green *= 1 - factor;
        blue *= 1 + factor;
      }

      auto r = static_cast<uint8_t>(red > 255 ? 255 : red);
      auto g = static_cast<uint8_t>(green > 255 ? 255 : green);
      auto b = static_cast<uint8_t>(blue > 255 ? 255 : blue);

      uint32_t color = (r << 16) | (g << 8) | (b << 0);

      full_nes_palette[i][j] = color;
    }
  }
}

void PPU::set_mirroring(int mode) { mirroring_mode = mode; }

void PPU::step()
{
  switch (ppu_state) {
    case Visible: scanline_cycle_visible(); break;
    case VBlank: scanline_cycle_nmi(); break;
    case PreRender: scanline_cycle_pre(); break;
    case Idle: break;
  }

  ++tick;
  if (tick > 340) {
    tick = 0;
    ++scanline;

    if (scanline == 240) {
      back_buffer = frame_buffer;
      std::transform(back_buffer.begin(), back_buffer.end(), back_buffer.begin(),
                     [this](uint32_t p) { return full_nes_palette[mask.rgb][p]; });
      ppu_state = Idle;
    } else if (scanline == 241) {
      ppu_state = VBlank;
    } else if (scanline > 241 && scanline < 261) {
      ppu_state = Idle;
    } else if (scanline == 261) {
      ppu_state = PreRender;
    }

    if (scanline > 261) {
      if (is_odd_frame && is_rendering) {
        tick = 1;  // Skip the first cycle of the visible scanline
      }

      ppu_state    = Visible;
      scanline     = 0;
      is_odd_frame = !is_odd_frame;
    }
  }
}

auto PPU::peek_reg(uint16_t addr) const -> uint8_t
{
  using namespace nes::types::ppu;

  switch (addr % 8) {
    case PPUSTATUS: return (bus_latch & 0x1F) | status.raw;
    case OAMDATA: return oam_mem[oam_addr];
    case PPUDATA:
      if (vram_addr.addr <= 0x3EFF) {
        return ppudata_buffer;
      } else {
        return peek_vram(vram_addr.addr);
      }
    default: break;
  }

  return bus_latch;
}

auto PPU::read(uint16_t addr) -> uint8_t
{
  switch (addr % 8) {
    case PPUSTATUS: {
      bus_latch     = (bus_latch & 0x1F) | status.raw;
      status.vblank = 0;
      addr_latch    = false;
      break;
    }

    case OAMDATA: {
      bus_latch = oam_mem[oam_addr];
      break;
    }

    case PPUDATA: {
      if (vram_addr.addr <= 0x3EFF) {
        bus_latch      = ppudata_buffer;
        ppudata_buffer = vram_read(vram_addr.addr);
      } else {
        ppudata_buffer = vram_read(vram_addr.addr);
        bus_latch      = ppudata_buffer;
      }

      vram_addr.addr += addr_increment;
    }

    default: break;
  }

  return bus_latch;
}

void PPU::write(uint16_t addr, uint8_t value)
{
  bus_latch = value;

  switch (addr % 8) {
    case PPUCTRL: {
      ctrl.raw     = value;
      temp_addr.nt = ctrl.nt;

      sprite_height  = ctrl.spr_size ? 16 : 8;
      addr_increment = ctrl.addr_inc ? 32 : 1;
      break;
    }

    case PPUMASK: {
      mask.raw = value;

      is_rendering     = mask.show_bg || mask.show_spr;
      grayscale_mask   = mask.grayscale ? 0x30 : 0x3F;
      selected_palette = mask.rgb;
      break;
    }

    case OAMADDR: {
      oam_addr = value;
      break;
    }

    case OAMDATA: {
      oam_mem[oam_addr] = value;
      ++oam_addr;
      break;
    }

    case PPUSCROLL: {
      if (!addr_latch) {  // First write
        fine_x             = value & 7;
        temp_addr.coarse_x = value >> 3;
      } else {  // Second write
        temp_addr.fine_y   = value & 7;
        temp_addr.coarse_y = value >> 3;
      }

      addr_latch = !addr_latch;
      break;
    }

    case PPUADDR: {
      if (!addr_latch) {  // First write
        temp_addr.h = value & 0x3F;
      } else {  // Second write
        temp_addr.l   = value;
        vram_addr.raw = temp_addr.raw;
      }

      addr_latch = !addr_latch;
      break;
    }

    case PPUDATA: {
      vram_write(vram_addr.addr, value);
      vram_addr.addr += addr_increment;
      break;
    }
  }
}

auto PPU::peek_vram(uint16_t addr) const -> uint8_t { return vram_read(addr); }

auto PPU::vram_read(uint16_t addr) const -> uint8_t
{
  switch (get_memory_map(addr)) {
    case CHR: return Cartridge::get().chr_read(addr);
    case Nametables: return ci_ram[nt_mirror_addr(addr)];
    case Palettes: return cg_ram[palette_addr(addr)] & grayscale_mask;
    default: return 0;
  }
}

void PPU::vram_write(uint16_t addr, uint8_t value)
{
  switch (get_memory_map(addr)) {
    case CHR: Cartridge::get().chr_write(addr, value); break;
    case Nametables: ci_ram[nt_mirror_addr(addr)] = value; break;
    case Palettes: cg_ram[palette_addr(addr)] = value; break;
  }
}

void PPU::clear_sec_oam() { sec_oam.fill({}); }

void PPU::sprite_evaluation()
{
  size_t size = 0;

  for (size_t i = 0; i < 64; ++i) {
    int line = scanline - oam_mem[i * 4];

    if (line >= 0 && line < sprite_height) {
      sec_oam[size].id   = i;
      sec_oam[size].y    = oam_mem[(i * 4) + 0];
      sec_oam[size].tile = oam_mem[(i * 4) + 1];
      sec_oam[size].attr = oam_mem[(i * 4) + 2];
      sec_oam[size].x    = oam_mem[(i * 4) + 3];

      ++size;

      if (size >= 8) {
        status.spr_overflow = true;
        return;
      }
    }
  }
}

void PPU::load_sprites()
{
  oam = sec_oam;

  for (auto& sprite : oam) {
    uint16_t addr = 0;

    // Sprite offset in the line
    uint8_t offset = (scanline - sprite.y) % sprite_height;

    // Vertical flip
    if (sprite.attr & 0x80) {
      offset ^= sprite_height - 1;
    }

    if (sprite_height == 8) {
      addr = (ctrl.spr_table * 0x1000) + (sprite.tile * 16);
      addr += offset;
    } else {
      addr = ((sprite.tile & 1) * 0x1000) + ((sprite.tile & 0xFE) * 16);
      addr += offset + (offset & 8);
    }

    // Fetch sprite data
    sprite.data_l = vram_read(addr + 0);
    sprite.data_h = vram_read(addr + 8);

    // Horizontal flip
    if (sprite.attr & 0x40) {
      // clang-format off
      constexpr auto lookup_table = std::to_array<uint8_t>({
          0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
          0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
          0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
          0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
          0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
          0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
          0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
          0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
          0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
          0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
          0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
          0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
          0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
          0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
          0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
          0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
      });
      // clang-format on

      sprite.data_l = lookup_table[sprite.data_l];
      sprite.data_h = lookup_table[sprite.data_h];
    }
  }
}

void PPU::horizontal_scroll()
{
  if (vram_addr.coarse_x == 31) {
    vram_addr.raw ^= 0x41F;
  } else {
    ++vram_addr.coarse_x;
  }
}

void PPU::vertical_scroll()
{
  if (vram_addr.fine_y < 7) {
    ++vram_addr.fine_y;
  } else {
    vram_addr.fine_y = 0;
    switch (vram_addr.coarse_y) {
      case 29:
        vram_addr.nt ^= 0b10;
        vram_addr.coarse_y = 0;
        break;
      case 31: vram_addr.coarse_y = 0; break;
      default: ++vram_addr.coarse_y; break;
    }
  }
}

void PPU::horizontal_update() { vram_addr.raw = (vram_addr.raw & ~0x041F) | (temp_addr.raw & 0x041F); }

void PPU::vertical_update() { vram_addr.raw = (vram_addr.raw & ~0x7BE0) | (temp_addr.raw & 0x7BE0); }

void PPU::background_shift()
{
  bg_shift_l <<= 1;
  bg_shift_h <<= 1;
  at_shift_l = (at_shift_l << 1) | at_latch_l;
  at_shift_h = (at_shift_h << 1) | at_latch_h;

  // Reload shift registers
  if (tick % 8 == 1) {
    bg_shift_l = (bg_shift_l & 0xFF00) | bg_latch_l;
    bg_shift_h = (bg_shift_h & 0xFF00) | bg_latch_h;

    at_latch >>= 2 * ((vram_addr.coarse_y & 0x02) | (((vram_addr.coarse_x - 1) & 0x02) >> 1));

    at_latch_l = at_latch & 1;
    at_latch_h = (at_latch >> 1) & 1;
  }
}

auto PPU::get_background_pixel() const -> uint8_t
{
  uint8_t pixel = tick - 2;

  if (mask.show_bg && !(!mask.bg_left && pixel < 8)) {
    uint8_t bg_palette = get_palette(bg_shift_l, bg_shift_h, 15 - fine_x);

    if (bg_palette) {
      uint8_t attr_palette = get_palette(at_shift_l, at_shift_h, 7 - fine_x);
      bg_palette |= attr_palette << 2;
    }

    return bg_palette;
  }

  return 0;
}

auto PPU::get_sprite_pixel() -> uint8_t
{
  uint8_t pixel      = tick - 2;
  uint8_t bg_palette = get_background_pixel();

  if (!(mask.show_spr && !(!mask.spr_left && pixel < 8))) return bg_palette;

  for (const auto& sprite : oam) {
    if (sprite.id == 0xFF) break;

    int offset = pixel - sprite.x;

    if (offset < 0 || offset >= 8) continue;  // Not in range

    uint8_t spr_palette = get_palette(sprite.data_l, sprite.data_h, 7 - offset);

    if (spr_palette != 0) {
      bool is_sprite0   = sprite.id == 0;
      bool spr_priority = !(sprite.attr & 0x20);
      spr_palette |= (sprite.attr & 3) << 2;
      spr_palette += 16;

      if (is_sprite0 && spr_palette && bg_palette && pixel != 255) {
        status.spr0_hit = true;
      }

      // Evaluate priority
      if (spr_palette && (spr_priority || bg_palette == 0)) return spr_palette;

      return bg_palette;
    }
  }

  return bg_palette;
}

void PPU::render_pixel()
{
  size_t row_pixel = tick - 2;
  size_t pixel_pos = (scanline * 256) + row_pixel;

  if (!is_rendering) {
    frame_buffer[pixel_pos] = vram_read(0x3F00);
    return;
  }

  frame_buffer[pixel_pos] = vram_read(0x3F00 + get_sprite_pixel());
}

void PPU::background_fetch()
{
  auto in_range = [this](int lower, int upper) { return (tick >= lower) && (tick <= upper); };

  if (in_range(2, 257) || in_range(322, 337)) {
    background_shift();
  }

  if (in_range(1, 255) || in_range(321, 338)) {
    switch (tick % 8) {
      case 1: ppu_addr = nt_addr(); break;
      case 2: nt_latch = vram_read(ppu_addr); break;

      case 3: ppu_addr = at_addr(); break;
      case 4: at_latch = vram_read(ppu_addr); break;

      case 5: ppu_addr = bg_addr(); break;
      case 6: bg_latch_l = vram_read(ppu_addr); break;

      case 7: ppu_addr += 8; break;
      case 0:
        bg_latch_h = vram_read(ppu_addr);
        horizontal_scroll();
        break;
    }
  } else {
    switch (tick) {
      case 256:
        bg_latch_h = vram_read(ppu_addr);
        vertical_scroll();
        break;

      case 257: horizontal_update(); break;

      case 339: ppu_addr = nt_addr(); break;
      case 340: nt_latch = vram_read(ppu_addr); break;

      default: break;
    }
  }
}

void PPU::scanline_cycle_pre()
{
  auto in_range = [this](int lower, int upper) { return (tick >= lower) && (tick <= upper); };

  if (tick == 1) {
    status.vblank       = false;
    status.spr_overflow = false;
    status.spr0_hit     = false;
  }

  if (is_rendering) {
    background_fetch();

    if (in_range(280, 304)) vertical_update();
    if (tick == 321) load_sprites();
    if (tick == 260) Cartridge::get().scanline_counter();
  }
}

void PPU::scanline_cycle_visible()
{
  if (tick >= 2 && tick <= 257) {
    render_pixel();
  }

  if (is_rendering) {
    background_fetch();

    switch (tick) {
      case 1: clear_sec_oam(); break;
      case 257: sprite_evaluation(); break;
      case 321: load_sprites(); break;
      default: break;
    }

    if (tick == 260) Cartridge::get().scanline_counter();
  }
}

void PPU::scanline_cycle_nmi()
{
  if (tick == 1) {
    status.vblank = true;

    if (ctrl.nmi) {
      nmi_conn->set(true);
    }
  }
}

//
// Auxiliary
//

auto PPU::nt_addr() const -> uint16_t { return 0x2000 | (vram_addr.raw & 0x0FFF); }

auto PPU::at_addr() const -> uint16_t
{
  return 0x23C0 | (vram_addr.nt << 10) | ((vram_addr.coarse_y / 4) << 3) | (vram_addr.coarse_x / 4);
}

auto PPU::bg_addr() const -> uint16_t { return (ctrl.bg_table * 0x1000) + (nt_latch * 16) + vram_addr.fine_y; }

auto PPU::nt_mirror_addr(uint16_t addr) const -> uint16_t
{
  switch (mirroring_conn->get()) {
    case Vertical: return addr & 0x07FF;
    case Horizontal: return ((addr >> 1) & 0x400) + (addr & 0x03FF);
    case One_Screen_Low: return addr & 0x03FF;
    case One_Screen_High: return 0x0400 + (addr & 0x03FF);
    case Four_Screen: return addr & 0x0FFF;
    default: throw std::runtime_error("Invalid mirroring type");
  }
}

auto PPU::palette_addr(uint16_t addr) const -> uint16_t
{
  return (((addr & 0x13) == 0x10) ? (addr & ~0x10) : addr) & 0x1F;
}

template <typename T> auto PPU::get_palette(T low, T high, int offset) const -> uint8_t
{
  constexpr auto nth_bit = [](auto x, auto n) -> uint8_t { return ((x >> n) & 1); };

  return nth_bit(high, offset) << 1 | nth_bit(low, offset);
}

//
// Snapshot
//

void PPU::save(std::ofstream& out)
{
  dump_snapshot(out, ci_ram);
  dump_snapshot(out, cg_ram);
  dump_snapshot(out, oam_mem);

  dump_snapshot(out, mirroring_mode);
  dump_snapshot(out, ppu_state, ppu_addr);
  dump_snapshot(out, scanline, tick, is_odd_frame);
  dump_snapshot(out, ctrl.raw, mask.raw, status.raw);
  dump_snapshot(out, bus_latch, ppudata_buffer, addr_latch);
  dump_snapshot(out, vram_addr.raw, temp_addr.raw, fine_x, oam_addr);

  dump_snapshot(out, nt_latch, at_latch, bg_latch_l, bg_latch_h, at_shift_l, at_shift_h, bg_shift_l, bg_shift_h,
                at_latch_l, at_latch_h);
}

void PPU::load(std::ifstream& in)
{
  get_snapshot(in, ci_ram);
  get_snapshot(in, cg_ram);
  get_snapshot(in, oam_mem);

  get_snapshot(in, mirroring_mode);
  get_snapshot(in, ppu_state, ppu_addr);
  get_snapshot(in, scanline, tick, is_odd_frame);
  get_snapshot(in, ctrl.raw, mask.raw, status.raw);
  get_snapshot(in, bus_latch, ppudata_buffer, addr_latch);
  get_snapshot(in, vram_addr.raw, temp_addr.raw, fine_x, oam_addr);

  get_snapshot(in, nt_latch, at_latch, bg_latch_l, bg_latch_h, at_shift_l, at_shift_h, bg_shift_l, bg_shift_h,
               at_latch_l, at_latch_h);

  // Restore auxiliary
  is_rendering     = mask.show_bg || mask.show_spr;
  grayscale_mask   = mask.grayscale ? 0x30 : 0x3F;
  selected_palette = mask.rgb;
  sprite_height    = ctrl.spr_size ? 16 : 8;
  addr_increment   = ctrl.addr_inc ? 32 : 1;

  // Evaluate and load sprites
  // Might be enough to restore the state
  sprite_evaluation();
  load_sprites();
}
}  // namespace nes
