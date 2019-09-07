#include "ppu.h"

#include <fstream>
#include <tuple>

#include "bit_utils.h"
#include "cartridge.h"
#include "cpu.h"
#include "emulator.h"
#include "log.h"
#include "utility/file_manager.h"

namespace nes {
ppu::ppu(emulator& emu_ref) : emu(emu_ref) {}

void ppu::power_on()
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

void ppu::reset()
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

std::array<uint32_t, 256 * 240> ppu::get_back_buffer() const
{
  return back_buffer;
}

void ppu::set_palette()
{
  std::ifstream palette(util::fmngr.get_palette_path(), std::ios::binary);

  if (!palette) {
    throw std::runtime_error("Couldn't open the palette file");
  }

  std::array<uint8_t, 64 * 3> pal_buffer;
  palette.read(reinterpret_cast<char*>(pal_buffer.data()), 64 * 3);

  for (size_t i = 0; i < 64; ++i) {
    uint8_t r = pal_buffer[(i * 3) + 0];
    uint8_t g = pal_buffer[(i * 3) + 1];
    uint8_t b = pal_buffer[(i * 3) + 2];

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

      uint8_t r = static_cast<uint8_t>(red > 255 ? 255 : red);
      uint8_t g = static_cast<uint8_t>(green > 255 ? 255 : green);
      uint8_t b = static_cast<uint8_t>(blue > 255 ? 255 : blue);

      uint32_t color = (r << 16) | (g << 8) | (b << 0);

      full_nes_palette[i][j] = color;
    }
  }
}

void ppu::set_mirroring(int mode)
{
  mirroring_mode = mode;
}

void ppu::step()
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
      // emu.get_io()->update_frame(frame_buffer);
      back_buffer = frame_buffer;
      ppu_state   = Idle;
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

uint8_t ppu::peek_reg(uint16_t addr) const
{
  using namespace nes::types::ppu::memory;

  switch (addr % 8) {
    case PPUSTATUS: return (bus_latch & 0x1F) | status.raw;
    case OAMDATA: return oam_mem[oam_addr];
    case PPUDATA:
      if (vram_addr.addr <= 0x3EFF) {
        return ppudata_buffer;
      } else {
        return peek_vram(vram_addr.addr);
      }
      break;
    default: break;
  }

  return bus_latch;
}

uint8_t ppu::read(uint16_t addr)
{
  using namespace nes::types::ppu::memory;

  switch (addr % 8) {
    case PPUSTATUS: {
      bus_latch     = (bus_latch & 0x1F) | status.raw;
      status.vblank = 0;
      addr_latch    = 0;
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

void ppu::write(uint16_t addr, uint8_t value)
{
  using namespace nes::types::ppu::memory;

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

      is_rendering   = mask.show_bg || mask.show_spr;
      grayscale_mask = mask.grayscale ? 0x30 : 0x3F;
      nes_to_rgb     = full_nes_palette[mask.rgb].data();
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

uint8_t ppu::peek_vram(uint16_t addr) const
{
  return vram_read(addr);
}

uint8_t ppu::vram_read(uint16_t addr) const
{
  using namespace nes::types::ppu::memory;

  switch (get_mem_map(addr)) {
    case CHR: return emu.get_cartridge()->chr_read(addr);
    case Nametables: return ci_ram[nt_mirror_addr(addr)];
    case Palettes: return cg_ram[palette_addr(addr)] & grayscale_mask;
    default: return 0;
  }
}

void ppu::vram_write(uint16_t addr, uint8_t value)
{
  using namespace nes::types::ppu::memory;

  switch (get_mem_map(addr)) {
    case CHR: emu.get_cartridge()->chr_write(addr, value); break;
    case Nametables: ci_ram[nt_mirror_addr(addr)] = value; break;
    case Palettes: cg_ram[palette_addr(addr)] = value; break;
  }
}

void ppu::clear_sec_oam()
{
  sec_oam.clear();
}

void ppu::sprite_evaluation()
{
  for (size_t i = 0; i < 64 && sec_oam.size() < 8; ++i) {
    int line = scanline - oam_mem[i * 4];

    if (line >= 0 && line < sprite_height) {
      sprite_info sprite;

      sprite.id   = i;
      sprite.y    = oam_mem[(i * 4) + 0];
      sprite.tile = oam_mem[(i * 4) + 1];
      sprite.attr = oam_mem[(i * 4) + 2];
      sprite.x    = oam_mem[(i * 4) + 3];

      sec_oam.push_back(sprite);

      if (sec_oam.size() == 8) {
        status.spr_overflow = true;
      }
    }
  }
}

void ppu::load_sprites()
{
  oam = sec_oam;

  for (auto& sprite : oam) {
    uint16_t addr;

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
      sprite.data_l = lib::reverse_byte(sprite.data_l);
      sprite.data_h = lib::reverse_byte(sprite.data_h);
    }
  }
}

void ppu::horizontal_scroll()
{
  if (vram_addr.coarse_x == 31) {
    vram_addr.raw ^= 0x41F;
  } else {
    ++vram_addr.coarse_x;
  }
}

void ppu::vertical_scroll()
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

void ppu::horizontal_update()
{
  vram_addr.raw = (vram_addr.raw & ~0x041F) | (temp_addr.raw & 0x041F);
}

void ppu::vertical_update()
{
  vram_addr.raw = (vram_addr.raw & ~0x7BE0) | (temp_addr.raw & 0x7BE0);
}

void ppu::background_shift()
{
  bg_shift_l <<= 1;
  bg_shift_h <<= 1;
  at_shift_l = (at_shift_l << 1) | at_latch_l;
  at_shift_h = (at_shift_h << 1) | at_latch_h;

  // Reload shift registers
  if (tick % 8 == 1) {
    bg_shift_l = (bg_shift_l & 0xFF00) | bg_latch_l;
    bg_shift_h = (bg_shift_h & 0xFF00) | bg_latch_h;

    at_latch >>= 2 * ((vram_addr.coarse_y & 0x02) |
                      (((vram_addr.coarse_x - 1) & 0x02) >> 1));

    at_latch_l = at_latch & 1;
    at_latch_h = (at_latch >> 1) & 1;
  }
}

auto ppu::get_sprite_pixel() const
{
  uint8_t pixel = static_cast<uint8_t>(tick - 2);

  if (mask.show_spr && !(!mask.spr_left && pixel < 8)) {
    for (const auto& sprite : oam) {
      int offset = pixel - sprite.x;

      if (offset < 0 || offset >= 8) continue;  // Not in range

      auto spr_palette = get_palette(sprite.data_l, sprite.data_h, 7 - offset);

      if (spr_palette != 0) {
        spr_palette |= (sprite.attr & 3) << 2;

        return std::tuple{
            sprite.id == 0,                          // Is sprite 0?
            static_cast<uint8_t>(spr_palette + 16),  // Sprite palette
            !(sprite.attr & 0x20)};                  // Is the sprite on top?
      }
    }
  }

  return std::tuple{false, uint8_t{0}, false};
}

auto ppu::get_background_pixel() const
{
  uint8_t pixel = static_cast<uint8_t>(tick - 2);

  if (mask.show_bg && !(!mask.bg_left && pixel < 8)) {
    auto bg_palette = get_palette(bg_shift_l, bg_shift_h, 15 - fine_x);

    if (bg_palette) {
      auto attr_palette = get_palette(at_shift_l, at_shift_h, 7 - fine_x);
      bg_palette |= attr_palette << 2;
    }

    return bg_palette;
  }

  return uint8_t{0};
}

void ppu::render_pixel()
{
  uint8_t pixel = static_cast<uint8_t>(tick - 2);

  if (!is_rendering) {
    frame_buffer[scanline * 256 + pixel] = nes_to_rgb[vram_read(0x3F00)];
    return;
  }

  auto bg_palette                              = get_background_pixel();
  auto [is_sprite0, spr_palette, spr_priority] = get_sprite_pixel();

  if (is_sprite0 && spr_palette && bg_palette && pixel != 255) {
    status.spr0_hit = true;
  }

  uint8_t palette = 0;

  // Evaluate priority
  if (spr_palette && (spr_priority || bg_palette == 0)) {
    palette = vram_read(0x3F00 + spr_palette);
  } else {
    palette = vram_read(0x3F00 + bg_palette);
  }

  frame_buffer[scanline * 256 + pixel] = nes_to_rgb[palette];
}

void ppu::background_fetch()
{
  auto in_range = [this](int lower, int upper) {
    return (tick >= lower) && (tick <= upper);
  };

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

void ppu::scanline_cycle_pre()
{
  auto in_range = [this](int lower, int upper) {
    return (tick >= lower) && (tick <= upper);
  };

  if (tick == 1) {
    status.vblank       = false;
    status.spr_overflow = false;
    status.spr0_hit     = false;
  }

  if (is_rendering) {
    background_fetch();

    if (in_range(280, 304)) vertical_update();
    if (tick == 321) load_sprites();
    if (tick == 260) emu.get_cartridge()->scanline_counter();
  }
}

void ppu::scanline_cycle_visible()
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

    if (tick == 260) emu.get_cartridge()->scanline_counter();
  }
}

void ppu::scanline_cycle_nmi()
{
  if (tick == 1) {
    status.vblank = true;

    if (ctrl.nmi) {
      emu.get_cpu()->set_nmi();
    }
  }
}

//
// Auxiliary
//

uint16_t ppu::nt_addr() const
{
  return 0x2000 | (vram_addr.raw & 0x0FFF);
}

uint16_t ppu::at_addr() const
{
  return 0x23C0 | (vram_addr.nt << 10) | ((vram_addr.coarse_y / 4) << 3) |
         (vram_addr.coarse_x / 4);
}

uint16_t ppu::bg_addr() const
{
  return (ctrl.bg_table * 0x1000) + (nt_latch * 16) + vram_addr.fine_y;
}

uint16_t ppu::nt_mirror_addr(uint16_t addr) const
{
  using namespace nes::mirroring;
  switch (mirroring_mode) {
    case Vertical: return addr & 0x07FF;
    case Horizontal: return ((addr >> 1) & 0x400) + (addr & 0x03FF);
    case One_Screen_Low: return addr & 0x03FF;
    case One_Screen_High: return 0x0400 + (addr & 0x03FF);
    case Four_Screen: return addr & 0x0FFF;
    default: throw std::runtime_error("Invalid mirroring type");
  }
}

uint16_t ppu::palette_addr(uint16_t addr) const
{
  return (((addr & 0x13) == 0x10) ? (addr & ~0x10) : addr) & 0x1F;
}

template <typename T> uint8_t ppu::get_palette(T low, T high, int offset) const
{
  constexpr auto nth_bit = [](auto x, auto n) -> uint8_t {
    return ((x >> n) & 1);
  };

  return nth_bit(high, offset) << 1 | nth_bit(low, offset);
}

//
// Snapshot
//

void ppu::save(std::ofstream& out)
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

  dump_snapshot(
      out,
      nt_latch,
      at_latch,
      bg_latch_l,
      bg_latch_h,
      at_shift_l,
      at_shift_h,
      bg_shift_l,
      bg_shift_h,
      at_latch_l,
      at_latch_h);
}

void ppu::load(std::ifstream& in)
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

  get_snapshot(
      in,
      nt_latch,
      at_latch,
      bg_latch_l,
      bg_latch_h,
      at_shift_l,
      at_shift_h,
      bg_shift_l,
      bg_shift_h,
      at_latch_l,
      at_latch_h);

  // Restore auxiliary
  is_rendering   = mask.show_bg || mask.show_spr;
  grayscale_mask = mask.grayscale ? 0x30 : 0x3F;
  nes_to_rgb     = full_nes_palette[mask.rgb].data();
  sprite_height  = ctrl.spr_size ? 16 : 8;
  addr_increment = ctrl.addr_inc ? 32 : 1;

  // Evaluate and load sprites
  // Might be enough to restore the state
  sprite_evaluation();
  load_sprites();
}
}  // namespace nes
