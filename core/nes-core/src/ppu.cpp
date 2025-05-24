#include "ppu.hpp"

#include <algorithm>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "cartridge.hpp"
#include "lib/common.hpp"
#include "types/ppu_types.hpp"

namespace nes {
auto Ppu::get() -> Ppu& {
  static Ppu instance;
  return instance;
}

void Ppu::power_on() {
  ctrl.raw = 0;
  mask.raw = 0;
  status.raw = 0;
  oam_addr = 0;
  addr_latch = false;

  vram_addr.raw = 0;
  temp_addr.raw = 0;
  fine_x = 0;
  oam_addr = 0;

  bus_latch = 0;
  ppudata_buffer = 0;

  is_odd_frame = false;
}

void Ppu::reset() {
  ctrl.raw = 0;
  mask.raw = 0;
  addr_latch = false;

  vram_addr.raw = 0;
  temp_addr.raw = 0;
  fine_x = 0;

  bus_latch = 0;
  ppudata_buffer = 0;

  is_odd_frame = false;
}

auto Ppu::get_frame_buffer() const -> const u32* {
  return frame_buffer.data();
}

void Ppu::set_palette(const std::vector<u8>& palette) {
  if (palette.size() != static_cast<usize>(64 * 3)) {
    throw std::invalid_argument("Invalid palette file");
  }

  for (usize i = 0; i < 64; ++i) {
    const auto r = palette[(i * 3) + 0];
    const auto g = palette[(i * 3) + 1];
    const auto b = palette[(i * 3) + 2];

    full_nes_palette[0][i] = static_cast<u32>(r << 16) | static_cast<u32>(g << 8) | static_cast<u32>(b);
  }

  // Generate full colour palette
  for (usize i = 1; i < 8; ++i) {
    for (usize j = 0; j < 64; ++j) {
      double red = static_cast<u8>(full_nes_palette[0][j] >> 16);
      double green = static_cast<u8>(full_nes_palette[0][j] >> 8);
      double blue = static_cast<u8>(full_nes_palette[0][j] >> 0);

      constexpr double factor = 0.3;

      // Intensify red
      if ((i & 0x01) != 0) {
        red *= 1 + factor;
        green *= 1 - factor;
        blue *= 1 - factor;
      }

      // Intensify green
      if ((i & 0x02) != 0) {
        red *= 1 - factor;
        green *= 1 + factor;
        blue *= 1 - factor;
      }

      // Intensify blue
      if ((i & 0x04) != 0) {
        red *= 1 - factor;
        green *= 1 - factor;
        blue *= 1 + factor;
      }

      const auto r = static_cast<u8>(red > 255 ? 255 : red);
      const auto g = static_cast<u8>(green > 255 ? 255 : green);
      const auto b = static_cast<u8>(blue > 255 ? 255 : blue);

      const u32 color = static_cast<u32>(r << 16) | static_cast<u32>(g << 8) | static_cast<u32>(b);

      full_nes_palette[i][j] = color;
    }
  }
}

void Ppu::step() {
  switch (ppu_state) {
    case Timing::Visible: scanline_cycle_visible(); break;
    case Timing::VBlank: scanline_cycle_nmi(); break;
    case Timing::PreRender: scanline_cycle_pre(); break;
    case Timing::Idle: break;

    default: unreachable();
  }

  ++tick;
  if (tick > 340) {
    tick = 0;
    ++scanline;

    if (scanline == 240) {
      frame_buffer = work_frame_buffer;
      std::ranges::transform(frame_buffer, frame_buffer.begin(), [this](const u32 p) {
        return full_nes_palette[mask.rgb()][p];
      });
      ppu_state = Timing::Idle;
    } else if (scanline == 241) {
      ppu_state = Timing::VBlank;
    } else if (scanline > 241 && scanline < 261) {
      ppu_state = Timing::Idle;
    } else if (scanline == 261) {
      ppu_state = Timing::PreRender;
    }

    if (scanline > 261) {
      if (is_odd_frame && is_rendering) {
        tick = 1; // Skip the first cycle of the visible scanline
      }

      ppu_state = Timing::Visible;
      scanline = 0;
      is_odd_frame = !is_odd_frame;
    }
  }
}

auto Ppu::peek_reg(const u16 addr) const -> u8 {
  using enum types::ppu::PpuMap;

  const auto map = static_cast<types::ppu::PpuMap>(addr % 8);

  switch (map) {
    case PpuStatus: return (bus_latch & 0x1F) | status.raw;
    case OamData: return oam_mem[oam_addr];
    case PpuData:
      if (vram_addr.addr() <= 0x3EFF) {
        return ppudata_buffer;
      }

      return peek_vram(vram_addr.addr());

    case PpuAddr:
    case PpuScroll:
    case PpuCtrl:
    case PpuMask:
    case OamAddr: break;

    default: unreachable();
  }

  return bus_latch;
}

auto Ppu::read(const u16 addr) -> u8 {
  using enum types::ppu::PpuMap;

  const auto map = static_cast<types::ppu::PpuMap>(addr % 8);

  switch (map) {
    case PpuStatus: {
      bus_latch = (bus_latch & 0x1F) | status.raw;
      status.set_vblank(false);
      addr_latch = false;
      break;
    }

    case OamData: {
      bus_latch = oam_mem[oam_addr];
      break;
    }

    case PpuData: {
      if (vram_addr.addr() <= 0x3EFF) {
        bus_latch = ppudata_buffer;
        ppudata_buffer = vram_read(vram_addr.addr());
      } else {
        ppudata_buffer = vram_read(vram_addr.addr());
        bus_latch = ppudata_buffer;
      }

      vram_addr.set_addr(vram_addr.addr() + addr_increment);

      break;
    }

    case PpuCtrl:
    case PpuMask:
    case OamAddr:
    case PpuScroll:
    case PpuAddr: {
      SPDLOG_CRITICAL("Unreachable");
      std::terminate();
    }

    default: unreachable();
  }

  return bus_latch;
}

void Ppu::write(const u16 addr, const u8 value) {
  using enum types::ppu::PpuMap;

  bus_latch = value;

  const auto map = static_cast<types::ppu::PpuMap>(addr % 8);

  switch (map) {
    case PpuCtrl: {
      ctrl.raw = value;
      temp_addr.set_nt(ctrl.nt());

      sprite_height = ctrl.spr_size() ? 16 : 8;
      addr_increment = ctrl.addr_inc() ? 32 : 1;
      break;
    }

    case PpuMask: {
      mask.raw = value;

      is_rendering = mask.show_bg() || mask.show_spr();
      grayscale_mask = mask.grayscale() ? 0x30 : 0x3F;
      selected_palette = mask.rgb();
      break;
    }

    case OamAddr: {
      oam_addr = value;
      break;
    }

    case OamData: {
      oam_mem[oam_addr] = value;
      ++oam_addr;
      break;
    }

    case PpuScroll: {
      if (!addr_latch) {
        // First write
        fine_x = value & 0b111;
        temp_addr.set_coarse_x(value >> 3);
      } else {
        // Second write
        temp_addr.set_fine_y(value & 0b111);
        temp_addr.set_coarse_y(value >> 3);
      }

      addr_latch = !addr_latch;
      break;
    }

    case PpuAddr: {
      if (!addr_latch) {
        // First write
        temp_addr.set_addr_high(value & 0b11'1111);
      } else {
        // Second write
        temp_addr.set_addr_low(value);
        vram_addr.raw = temp_addr.raw;
      }

      addr_latch = !addr_latch;
      break;
    }

    case PpuData: {
      vram_write(vram_addr.addr(), value);
      vram_addr.set_addr(vram_addr.addr() + addr_increment);
      break;
    }

    case PpuStatus:
    default: {
      SPDLOG_CRITICAL("Unreachable");
      std::terminate();
    }
  }
}

auto Ppu::peek_vram(const u16 addr) const -> u8 {
  return vram_read(addr);
}

auto Ppu::vram_read(const u16 addr) const -> u8 {
  using enum types::ppu::MemoryMap;

  switch (types::ppu::get_memory_map(addr)) {
    case Chr: return Cartridge::get().chr_read(addr);
    case Nametables: return ci_ram[nt_mirror_addr(addr)];
    case Palettes: return cg_ram[palette_addr(addr)] & grayscale_mask;
    case Unknown: return 0;

    default: unreachable();
  }
}

void Ppu::vram_write(const u16 addr, const u8 value) {
  using enum types::ppu::MemoryMap;

  switch (types::ppu::get_memory_map(addr)) {
    case Chr: Cartridge::get().chr_write(addr, value); break;
    case Nametables: ci_ram[nt_mirror_addr(addr)] = value; break;
    case Palettes: cg_ram[palette_addr(addr)] = value; break;
    case Unknown: throw std::runtime_error("Unreachable");

    default: unreachable();
  }
}

void Ppu::clear_sec_oam() {
  sec_oam.fill({});
}

void Ppu::sprite_evaluation() {
  usize size = 0;

  for (usize i = 0; i < 64; ++i) {
    const i32 line = scanline - oam_mem[i * 4];

    if (line < 0) {
      continue;
    }

    if (static_cast<u8>(line) < sprite_height) {
      sec_oam[size].id = i;
      sec_oam[size].y = oam_mem[(i * 4) + 0];
      sec_oam[size].tile = oam_mem[(i * 4) + 1];
      sec_oam[size].attr = oam_mem[(i * 4) + 2];
      sec_oam[size].x = oam_mem[(i * 4) + 3];

      ++size;

      if (size >= 8) {
        status.set_spr_overflow(true);
        return;
      }
    }
  }
}

void Ppu::load_sprites() {
  oam = sec_oam;

  for (auto& sprite : oam) {
    u16 addr = 0;

    // Sprite offset in the line
    u8 offset = (scanline - sprite.y) % sprite_height;

    // Vertical flip
    if ((sprite.attr & 0x80) != 0) {
      offset ^= sprite_height - 1;
    }

    if (sprite_height == 8) {
      addr = (static_cast<u16>(ctrl.spr_table()) * 0x1000) + (sprite.tile * 16);
      addr += offset;
    } else {
      addr = ((sprite.tile & 1) * 0x1000) + ((sprite.tile & 0xFE) * 16);
      addr += offset + (offset & 8);
    }

    // Fetch sprite data
    sprite.data_l = vram_read(addr + 0);
    sprite.data_h = vram_read(addr + 8);

    // Horizontal flip
    if ((sprite.attr & 0x40) != 0) {
      // clang-format off
      constexpr auto lookup_table = std::to_array<u8>({
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

void Ppu::horizontal_scroll() {
  const auto coarse_x = vram_addr.coarse_x();
  if (coarse_x == 31) {
    vram_addr.raw ^= 0x41F;
  } else {
    vram_addr.set_coarse_x(coarse_x + 1);
  }
}

void Ppu::vertical_scroll() {
  if (vram_addr.fine_y() < 7) {
    vram_addr.set_fine_y(vram_addr.fine_y() + 1);
  } else {
    vram_addr.set_fine_y(0);
    switch (vram_addr.coarse_y()) {
      case 29:
        vram_addr.set_nt(vram_addr.nt() ^ 0b10);
        vram_addr.set_coarse_y(0);
        break;
      case 31: vram_addr.set_coarse_y(0); break;
      default: vram_addr.set_coarse_y(vram_addr.coarse_y() + 1); break;
    }
  }
}

void Ppu::horizontal_update() {
  vram_addr.raw = (vram_addr.raw & ~0x041Fu) | (temp_addr.raw & 0x041Fu);
}

void Ppu::vertical_update() {
  vram_addr.raw = (vram_addr.raw & ~0x7BE0u) | (temp_addr.raw & 0x7BE0u);
}

void Ppu::background_shift() {
  bg_shift_l <<= 1;
  bg_shift_h <<= 1;
  at_shift_l = static_cast<u8>(at_shift_l << 1) | at_latch_l;
  at_shift_h = static_cast<u8>(at_shift_h << 1) | at_latch_h;

  // Reload shift registers
  if (tick % 8 == 1) {
    bg_shift_l = (bg_shift_l & 0xFF00) | bg_latch_l;
    bg_shift_h = (bg_shift_h & 0xFF00) | bg_latch_h;

    at_latch >>= 2 * ((vram_addr.coarse_y() & 0x02) | (((vram_addr.coarse_x() - 1) & 0x02) >> 1));

    at_latch_l = at_latch & 1;
    at_latch_h = (at_latch >> 1) & 1;
  }
}

auto Ppu::get_background_pixel() const -> u8 {
  const auto pixel = static_cast<u8>(tick - 2);

  if (mask.show_bg() && (mask.bg_left() || pixel >= 8)) {
    u8 bg_palette = get_palette(bg_shift_l, bg_shift_h, 15 - fine_x);

    if (bg_palette != 0) {
      const auto attr_palette = get_palette(at_shift_l, at_shift_h, 7 - fine_x);
      bg_palette |= attr_palette << 2;
    }

    return bg_palette;
  }

  return 0;
}

auto Ppu::get_sprite_pixel() -> u8 {
  const auto pixel = static_cast<u8>(tick - 2);
  const auto bg_palette = get_background_pixel();

  if (!mask.show_spr() || (!mask.spr_left() && pixel < 8)) {
    return bg_palette;
  }

  for (const auto& sprite : oam) {
    if (sprite.id == 0xFF) {
      break;
    }

    const i32 offset = pixel - sprite.x;

    if (offset < 0 || offset >= 8) {
      continue; // Not in range
    }

    u8 spr_palette = get_palette(sprite.data_l, sprite.data_h, static_cast<u8>(7 - offset));

    if (spr_palette != 0) {
      const auto is_sprite0 = sprite.id == 0;
      const auto spr_priority = (sprite.attr & 0x20) == 0;
      spr_palette |= (sprite.attr & 3) << 2;
      spr_palette += 16;

      if (is_sprite0 && (spr_palette != 0) && (bg_palette != 0) && pixel != 255) {
        status.set_spr0_hit(true);
      }

      // Evaluate priority
      if ((spr_palette != 0) && (spr_priority || bg_palette == 0)) {
        return spr_palette;
      }

      return bg_palette;
    }
  }

  return bg_palette;
}

void Ppu::render_pixel() {
  const usize row_pixel = tick - 2;
  const usize pixel_pos = static_cast<usize>(scanline * 256u) + row_pixel;

  if (!is_rendering) {
    work_frame_buffer[pixel_pos] = vram_read(0x3F00);
    return;
  }

  work_frame_buffer[pixel_pos] = vram_read(0x3F00 + get_sprite_pixel());
}

void Ppu::background_fetch() {
  auto in_range = [this](const auto lower, const auto upper) { return (tick >= lower) && (tick <= upper); };

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

      default: unreachable();
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

void Ppu::scanline_cycle_pre() {
  auto in_range = [this](const auto lower, const auto upper) { return (tick >= lower) && (tick <= upper); };

  if (tick == 1) {
    status.set_vblank(false);
    status.set_spr_overflow(false);
    status.set_spr0_hit(false);
  }

  if (is_rendering) {
    background_fetch();

    if (in_range(280, 304)) {
      vertical_update();
    }
    if (tick == 321) {
      load_sprites();
    }
    if (tick == 260) {
      Cartridge::get().scanline_counter();
    }
  }
}

void Ppu::scanline_cycle_visible() {
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

    if (tick == 260) {
      Cartridge::get().scanline_counter();
    }
  }
}

void Ppu::scanline_cycle_nmi() {
  if (tick == 1) {
    status.set_vblank(true);

    if (ctrl.nmi()) {
      *nmi_conn = true;
    }
  }
}

//
// Auxiliary
//

auto Ppu::nt_addr() const -> u16 {
  return 0x2000 | (vram_addr.raw & 0x0FFF);
}

auto Ppu::at_addr() const -> u16 {
  return 0x23C0
    | static_cast<u16>(vram_addr.nt() << 10)
    | static_cast<u16>((vram_addr.coarse_y() / 4) << 3)
    | static_cast<u16>(vram_addr.coarse_x() / 4);
}

auto Ppu::bg_addr() const -> u16 {
  return (static_cast<u16>(ctrl.bg_table()) * 0x1000) + (nt_latch * 16) + vram_addr.fine_y();
}

auto Ppu::nt_mirror_addr(const u16 addr) const -> u16 {
  using enum types::ppu::MirroringType;

  switch (Cartridge::get().get_mirroring()) {
    case Vertical: return addr & 0x07FF;
    case Horizontal: return ((addr >> 1) & 0x400) + (addr & 0x03FF);
    case OneScreenLow: return addr & 0x03FF;
    case OneScreenHigh: return 0x0400 + (addr & 0x03FF);
    case FourScreen: return addr & 0x0FFF;
    case Unknown: throw std::runtime_error("Invalid mirroring type");

    default: unreachable();
  }
}

auto Ppu::palette_addr(const u16 addr) -> u16 {
  return (((addr & 0x13) == 0x10) ? (addr & ~0x10) : addr) & 0x1F;
}

template <typename T>
auto Ppu::get_palette(T low, T high, u8 offset) const -> u8 {
  constexpr auto nth_bit = [](auto x, auto n) -> u8 { return ((x >> n) & 1); };

  return static_cast<u8>(nth_bit(high, offset) << 1) | nth_bit(low, offset);
}
} // namespace nes
