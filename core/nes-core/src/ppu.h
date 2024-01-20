#pragma once

#include <array>
#include <memory>

#include <common.h>
#include "types/ppu_types.h"
#include "utility/snapshotable.hpp"

namespace nes {
class Ppu final : public utility::Snapshotable {
public:
  Ppu(const Ppu&) = delete;
  Ppu(Ppu&&) = delete;
  auto operator=(const Ppu&) -> Ppu& = delete;
  auto operator=(Ppu&&) -> Ppu& = delete;

  static auto get() -> Ppu&;

  void power_on();
  void reset();

  auto get_back_buffer() const -> const uint32_t*;

  void set_palette();

  auto read(uint16_t) -> uint8_t;
  void write(uint16_t, uint8_t);

  void step();

  std::shared_ptr<bool> nmi_conn;
  std::shared_ptr<types::ppu::MirroringType> mirroring_conn;

  //
  // Read without side effects
  //

  auto cycle_count() const -> int { return tick; }

  auto scanline_count() const -> int { return scanline; }

  auto peek_reg(uint16_t) const -> uint8_t;
  auto peek_vram(uint16_t) const -> uint8_t;

  //
  // Snapshot
  //

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  Ppu() = default;

  //
  // VRAM access
  //

  auto vram_read(uint16_t) const -> uint8_t;
  void vram_write(uint16_t, uint8_t);

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

  auto get_background_pixel() const -> uint8_t;
  auto get_sprite_pixel() -> uint8_t;

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

  auto nt_addr() const -> uint16_t;               // Nametable address
  auto at_addr() const -> uint16_t;               // Attribute address
  auto bg_addr() const -> uint16_t;               // Background address
  auto palette_addr(uint16_t) const -> uint16_t;  // Palette address
  auto nt_mirror_addr(uint16_t) const
    -> uint16_t;  // Relative nametable address

  template <typename T>
  auto get_palette(T, T, int) const -> uint8_t;  // Get palette

  enum timing { Idle, Visible, VBlank, PreRender };

  int ppu_state = Visible;
  uint16_t ppu_addr = 0;

  //
  // Register access
  //

  uint8_t bus_latch = 0;       // Register access buffer
  uint8_t ppudata_buffer = 0;  // PPUDATA read buffer
  bool addr_latch = false;     // Address latch used by PPUSCROLL and PPUADDR

  using sprite_info = types::ppu::sprite_info;
  using loopy_addr = types::ppu::loopy_addr;
  using ppuctrl = types::ppu::ppuctrl;
  using ppumask = types::ppu::ppumask;
  using ppustatus = types::ppu::ppustatus;

  using ci_ram_type = std::array<uint8_t, 0x800>;
  using cg_ram_type = std::array<uint8_t, 0x20>;
  using oam_mem_type = std::array<uint8_t, 0x100>;

  using oam_type = std::array<sprite_info, 8>;
  using sec_oam_type = oam_type;

  using frame_buffer_type = std::array<uint32_t, 256 * 240>;

  using full_nes_palette_type = std::array<std::array<uint32_t, 64>, 8>;

  ci_ram_type ci_ram = {};    // Console-Internal RAM
  cg_ram_type cg_ram = {};    // Colour generator RAM
  oam_mem_type oam_mem = {};  // Object Attribute Memory (sprites)

  oam_type oam = {};          // Sprite buffer
  sec_oam_type sec_oam = {};  // Secondary sprite buffer

  frame_buffer_type frame_buffer = {};  // Frame buffer
  frame_buffer_type back_buffer = {};   // Back buffer

  full_nes_palette_type full_nes_palette = {};
  uint8_t selected_palette = 0;
  // const uint32_t* nes_to_rgb = full_nes_palette[0].data();

  loopy_addr vram_addr;
  loopy_addr temp_addr;
  uint8_t fine_x = 0;
  uint8_t oam_addr = 0;

  ppuctrl ctrl;      // PPUCTRL   ($2000) register
  ppumask mask;      // PPUMASK   ($2001) register
  ppustatus status;  // PPUSTATUS ($2002) register

  //
  // Background latches
  //

  uint8_t nt_latch = 0;    // Nametable latch
  uint8_t at_latch = 0;    // Attribute latch
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

  int scanline = 0;           // Scanline counter
  int tick = 0;               // Cycle counter
  bool is_odd_frame = false;  // Is an odd frame?

  //
  // Auxiliary
  //

  bool is_rendering = false;
  uint8_t sprite_height = 8;
  uint8_t addr_increment = 1;
  uint8_t grayscale_mask = 0x3F;
};
}  // namespace nes
