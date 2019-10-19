#pragma once

#include <array>

#include <common.h>
#include "Types/PPU_Types.h"
#include "Utility/Snapshotable.hpp"

namespace nes {
class ppu : public util::snapshotable {
public:
  ppu(const ppu&) = delete;
  ppu(ppu&&)      = delete;
  ppu& operator=(const ppu&) = delete;
  ppu& operator=(ppu&&) = delete;

  static ppu& get();

  void power_on();
  void reset();

  std::array<uint32_t, 256 * 240> get_back_buffer() const;

  void set_palette();
  void set_mirroring(int);

  uint8_t read(uint16_t);
  void    write(uint16_t, uint8_t);

  void step();

  //
  // Read without side effects
  //

  int cycle_count() const { return tick; }

  int scanline_count() const { return scanline; }

  uint8_t peek_reg(uint16_t) const;
  uint8_t peek_vram(uint16_t) const;

  //
  // Snapshot
  //

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  ppu() = default;

  //
  // VRAM access
  //

  uint8_t vram_read(uint16_t) const;
  void    vram_write(uint16_t, uint8_t);

  //
  // Sprites
  //

  void clear_sec_oam();      // Clear secondary OAM
  void sprite_evaluation();  // Fill the secondary OAM with new sprites
  void load_sprites();       // Load the sprites into the primary OAM

  //
  // Scrolling
  //

  void horizontal_scroll();  // Horizontal scrolling
  void vertical_scroll();    // Vertical scrolling

  void horizontal_update();  // Update loopy V from loopy T
  void vertical_update();    // Update loopy V from loopy T

  //
  // Rendering
  //

  auto get_background_pixel() const;
  auto get_sprite_pixel() const;

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

  uint16_t nt_addr() const;                 // Nametable address
  uint16_t at_addr() const;                 // Attribute address
  uint16_t bg_addr() const;                 // Background address
  uint16_t palette_addr(uint16_t) const;    // Palette address
  uint16_t nt_mirror_addr(uint16_t) const;  // Relative nametable address

  template <typename T> uint8_t get_palette(T, T, int) const;  // Get palette

  enum timing { Idle, Visible, VBlank, PreRender };

  int      ppu_state = Visible;
  uint16_t ppu_addr  = 0;

  //
  // Register access
  //

  uint8_t bus_latch      = 0;  // Register access buffer
  uint8_t ppudata_buffer = 0;  // PPUDATA read buffer
  bool    addr_latch = false;  // Address latch used by PPUSCROLL and PPUADDR

  int mirroring_mode = 0;

  using sprite_info = types::ppu::sprite_info;
  using loopy_addr  = types::ppu::loopy_addr;
  using ppuctrl     = types::ppu::ppuctrl;
  using ppumask     = types::ppu::ppumask;
  using ppustatus   = types::ppu::ppustatus;

  std::array<uint8_t, 0x800> ci_ram  = {};  // Console-Internal RAM
  std::array<uint8_t, 0x20>  cg_ram  = {};  // Colour generator RAM
  std::array<uint8_t, 0x100> oam_mem = {};  // Object Attribute Memory (sprites)

  std::array<sprite_info, 8> oam     = {};  // Sprite buffer
  std::array<sprite_info, 8> sec_oam = {};  // Secondary sprite buffer

  std::array<uint32_t, 256 * 240> frame_buffer = {};  // Frame buffer
  std::array<uint32_t, 256 * 240> back_buffer  = {};  // Back buffer

  std::array<std::array<uint32_t, 64>, 8> full_nes_palette = {};
  const uint32_t* nes_to_rgb = full_nes_palette[0].data();

  loopy_addr vram_addr;
  loopy_addr temp_addr;
  uint8_t    fine_x   = 0;
  uint8_t    oam_addr = 0;

  ppuctrl   ctrl;    // PPUCTRL   ($2000) register
  ppumask   mask;    // PPUMASK   ($2001) register
  ppustatus status;  // PPUSTATUS ($2002) register

  //
  // Background latches
  //

  uint8_t nt_latch   = 0;  // Nametable latch
  uint8_t at_latch   = 0;  // Attribute latch
  uint8_t bg_latch_l = 0;  // Background latch (low)
  uint8_t bg_latch_h = 0;  // Background latch (high)

  uint8_t at_shift_l = 0;  // Attribute shift register (low)
  uint8_t at_shift_h = 0;  // Attribute shift register (high)

  uint16_t bg_shift_l = 0;  // Background shift register (low)
  uint16_t bg_shift_h = 0;  // Background shift register (high)

  uint8_t at_latch_l = 0;  // Attribute latch (low)
  uint8_t at_latch_h = 0;  // Attribute latch (high)

  //
  // Rendering counters
  //

  int  scanline     = 0;      // Scanline counter
  int  tick         = 0;      // Cycle counter
  bool is_odd_frame = false;  // Is an odd frame?

  //
  // Auxiliary
  //

  bool    is_rendering   = false;
  uint8_t sprite_height  = 8;
  uint8_t addr_increment = 1;
  uint8_t grayscale_mask = 0x3F;
};
}  // namespace nes