#pragma once

#include <array>
#include <memory>

#include "lib/common.hpp"
#include "types/ppu_types.hpp"

namespace nes {
class Ppu final {
public:
  static auto get() -> Ppu&;

  void power_on();
  void reset();

  [[nodiscard]] auto get_frame_buffer() const -> const u32*;

  void set_palette();

  auto read(u16) -> u8;
  void write(u16, u8);

  void step();

  std::shared_ptr<bool> nmi_conn;
  std::shared_ptr<types::ppu::MirroringType> mirroring_conn;

  //
  // Read without side effects
  //

  [[nodiscard]] auto cycle_count() const -> i32 {
    return tick;
  }

  [[nodiscard]] auto scanline_count() const -> i32 {
    return scanline;
  }

  [[nodiscard]] auto peek_reg(u16) const -> u8;
  [[nodiscard]] auto peek_vram(u16) const -> u8;

private:
  Ppu() = default;

  //
  // VRAM access
  //

  [[nodiscard]] auto vram_read(u16) const -> u8;
  void vram_write(u16, u8);

  //
  // Sprites
  //

  void clear_sec_oam();     // Clear secondary OAM
  void sprite_evaluation(); // Fill the secondary OAM with new sprites
  void load_sprites();      // Load the sprites into the primary OAM

  //
  // Scrolling
  //

  void horizontal_scroll(); // Horizontal scrolling
  void vertical_scroll();   // Vertical scrolling

  void horizontal_update(); // Update loopy V from loopy T
  void vertical_update();   // Update loopy V from loopy T

  //
  // Rendering
  //

  [[nodiscard]] auto get_background_pixel() const -> u8;
  [[nodiscard]] auto get_sprite_pixel() -> u8;

  void render_pixel();

  //
  // Scanline cycles and background operations
  //

  void background_fetch();
  void background_shift();

  void scanline_cycle_pre();
  void scanline_cycle_visible();
  void scanline_cycle_nmi();

  //
  // Addresses
  //

  [[nodiscard]] auto nt_addr() const -> u16;           // Nametable address
  [[nodiscard]] auto at_addr() const -> u16;           // Attribute address
  [[nodiscard]] auto bg_addr() const -> u16;           // Background address
  [[nodiscard]] static auto palette_addr(u16) -> u16;  // Palette address
  [[nodiscard]] auto nt_mirror_addr(u16) const -> u16; // Relative nametable address

  template <typename T>
  [[nodiscard]] auto get_palette(T, T, i32) const -> u8; // Get palette

  enum Timing {
    Idle,
    Visible,
    VBlank,
    PreRender
  };

  Timing ppu_state = Visible;
  u16 ppu_addr = 0;

  //
  // Register access
  //

  u8 bus_latch = 0;        // Register access buffer
  u8 ppudata_buffer = 0;   // PPUDATA read buffer
  bool addr_latch = false; // Address latch used by PPUSCROLL and PPUADDR

  using SpriteInfo = types::ppu::SpriteInfo;
  using LoopyAddr = types::ppu::LoopyAddr;
  using Ppuctrl = types::ppu::Ppuctrl;
  using Ppumask = types::ppu::Ppumask;
  using Ppustatus = types::ppu::Ppustatus;

  using ci_ram_type = std::array<u8, 0x800>;
  using cg_ram_type = std::array<u8, 0x20>;
  using oam_mem_type = std::array<u8, 0x100>;

  using oam_type = std::array<SpriteInfo, 8>;
  using sec_oam_type = oam_type;

  using frame_buffer_type = std::array<u32, 256 * 240>;

  using full_nes_palette_type = std::array<std::array<u32, 64>, 8>;

  ci_ram_type ci_ram = {};   // Console-Internal RAM
  cg_ram_type cg_ram = {};   // Colour generator RAM
  oam_mem_type oam_mem = {}; // Object Attribute Memory (sprites)

  oam_type oam = {};         // Sprite buffer
  sec_oam_type sec_oam = {}; // Secondary sprite buffer

  frame_buffer_type frame_buffer = {};      // Frame buffer
  frame_buffer_type work_frame_buffer = {}; // Back buffer

  full_nes_palette_type full_nes_palette = {};
  u8 selected_palette = 0;
  // const u32* nes_to_rgb = full_nes_palette[0].data();

  LoopyAddr vram_addr;
  LoopyAddr temp_addr;
  u8 fine_x = 0;
  u8 oam_addr = 0;

  Ppuctrl ctrl;     // PPUCTRL   ($2000) register
  Ppumask mask;     // PPUMASK   ($2001) register
  Ppustatus status; // PPUSTATUS ($2002) register

  //
  // Background latches
  //

  u8 nt_latch = 0;   // Nametable latch
  u8 at_latch = 0;   // Attribute latch
  u8 bg_latch_l = 0; // Background latch (low)
  u8 bg_latch_h = 0; // Background latch (high)

  u8 at_shift_l = 0; // Attribute shift register (low)
  u8 at_shift_h = 0; // Attribute shift register (high)

  u16 bg_shift_l = 0; // Background shift register (low)
  u16 bg_shift_h = 0; // Background shift register (high)

  u8 at_latch_l = 0; // Attribute latch (low)
  u8 at_latch_h = 0; // Attribute latch (high)

  //
  // Rendering counters
  //

  u16 scanline = 0;          // Scanline counter
  u16 tick = 0;              // Cycle counter
  bool is_odd_frame = false; // Is an odd frame?

  //
  // Auxiliary
  //

  bool is_rendering = false;
  u8 sprite_height = 8;
  u8 addr_increment = 1;
  u8 grayscale_mask = 0x3F;
};
} // namespace nes
