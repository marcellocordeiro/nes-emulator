#pragma once

#include <common.h>
#include <bit_field.hpp>

namespace nes::types::ppu {
template <size_t position, size_t bits> using bf_8 = lib::bit_field<uint8_t, position, bits>;

template <size_t position, size_t bits> using bf_16 = lib::bit_field<uint16_t, position, bits>;

// PPUCTRL ($2000) register
union ppuctrl {
  uint8_t raw = 0;

  bf_8<0, 2> nt;         // Base nametable addr [$2000, $2400, $2800, $2C00]
  bf_8<2, 1> addr_inc;   // Address increment [1, 32]
  bf_8<3, 1> spr_table;  // Sprite pattern table [$0000, $1000]
  bf_8<4, 1> bg_table;   // Background pattern table [$0000, $1000]
  bf_8<5, 1> spr_size;   // Sprite size [8x8, 8x16]
  bf_8<7, 1> nmi;        // Enable NMI on the next VBlank

  // bf_8<6, 1> slave;  // PPU master/slave (unused)
};

// PPUMASK ($2001) register
union ppumask {
  uint8_t raw = 0;

  bf_8<0, 1> grayscale;  // Grayscale
  bf_8<1, 1> bg_left;    // Show background in the first column (8 pixels)
  bf_8<2, 1> spr_left;   // Show sprite in the first column (8 pixels)
  bf_8<3, 1> show_bg;    // Show background
  bf_8<4, 1> show_spr;   // Show sprites
  bf_8<5, 3> rgb;        // Palette picker (colour emphasis)
};

// PPUSTATUS ($2002) register
union ppustatus {
  uint8_t raw = 0;

  bf_8<5, 1> spr_overflow;  // Sprite overflow
  bf_8<6, 1> spr0_hit;      // Sprite 0 hit
  bf_8<7, 1> vblank;        // In VBlank?
};

// Loopy's VRAM address
union loopy_addr {
  uint16_t raw = 0;

  bf_16<0, 5>  coarse_x;  // Coarse X
  bf_16<5, 5>  coarse_y;  // Coarse Y
  bf_16<10, 2> nt;        // Nametable
  bf_16<12, 3> fine_y;    // Fine Y

  bf_16<0, 8> l;  // Low addr part
  bf_16<8, 7> h;  // High addr part

  bf_16<0, 14> addr;
};

// Sprite info
struct sprite_info {
  uint8_t y    = 64;    // Y position
  uint8_t tile = 0xFF;  // Tile index
  uint8_t attr = 0xFF;  // Attributes
  uint8_t x    = 0xFF;  // X position

  size_t  id     = 0xFF;  // Index in OAM
  uint8_t data_l = 0;     // Tile data (low)
  uint8_t data_h = 0;     // Tile data (high)
};

namespace memory {
constexpr int Unknown = -1;

enum ppu_map {
  PPUCTRL   = 0,
  PPUMASK   = 1,
  PPUSTATUS = 2,
  OAMADDR   = 3,
  OAMDATA   = 4,
  PPUSCROLL = 5,
  PPUADDR   = 6,
  PPUDATA   = 7
};

enum mem_map { CHR, Nametables, Palettes };

template <auto Operation> int get_map(uint16_t);
int                           get_mem_map(uint16_t);
}  // namespace memory
}  // namespace nes::types::ppu
