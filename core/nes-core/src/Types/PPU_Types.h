#pragma once

#include <string_view>

#include <common.h>
#include <bit_field.hpp>

namespace nes::types::ppu {
template <size_t position, size_t bits>
using BitFieldU8 = lib::BitField<uint8_t, position, bits>;

template <size_t position, size_t bits>
using BitFieldU16 = lib::BitField<uint16_t, position, bits>;

// PPUCTRL ($2000) register
union ppuctrl {
  uint8_t raw = 0;

  BitFieldU8<0, 2> nt;  // Base nametable addr [$2000, $2400, $2800, $2C00]
  BitFieldU8<2, 1> addr_inc;   // Address increment [1, 32]
  BitFieldU8<3, 1> spr_table;  // Sprite pattern table [$0000, $1000]
  BitFieldU8<4, 1> bg_table;   // Background pattern table [$0000, $1000]
  BitFieldU8<5, 1> spr_size;   // Sprite size [8x8, 8x16]
  BitFieldU8<7, 1> nmi;        // Enable NMI on the next VBlank

  // BitFieldU8<6, 1> slave;  // PPU master/slave (unused)
};

// PPUMASK ($2001) register
union ppumask {
  uint8_t raw = 0;

  BitFieldU8<0, 1> grayscale;  // Grayscale
  BitFieldU8<1, 1> bg_left;    // Show background in the first column (8 pixels)
  BitFieldU8<2, 1> spr_left;   // Show sprite in the first column (8 pixels)
  BitFieldU8<3, 1> show_bg;    // Show background
  BitFieldU8<4, 1> show_spr;   // Show sprites
  BitFieldU8<5, 3> rgb;        // Palette picker (colour emphasis)
};

// PPUSTATUS ($2002) register
union ppustatus {
  uint8_t raw = 0;

  BitFieldU8<5, 1> spr_overflow;  // Sprite overflow
  BitFieldU8<6, 1> spr0_hit;      // Sprite 0 hit
  BitFieldU8<7, 1> vblank;        // In VBlank?
};

// Loopy's VRAM address
union loopy_addr {
  uint16_t raw = 0;

  BitFieldU16<0, 5> coarse_x;  // Coarse X
  BitFieldU16<5, 5> coarse_y;  // Coarse Y
  BitFieldU16<10, 2> nt;       // Nametable
  BitFieldU16<12, 3> fine_y;   // Fine Y

  BitFieldU16<0, 8> l;  // Low addr part
  BitFieldU16<8, 7> h;  // High addr part

  BitFieldU16<0, 14> addr;
};

// Sprite info
struct sprite_info {
  uint8_t y = 64;       // Y position
  uint8_t tile = 0xFF;  // Tile index
  uint8_t attr = 0xFF;  // Attributes
  uint8_t x = 0xFF;     // X position

  size_t id = 0xFF;    // Index in OAM
  uint8_t data_l = 0;  // Tile data (low)
  uint8_t data_h = 0;  // Tile data (high)
};

enum PpuMap {
  PpuCtrl = 0,
  PpuMask = 1,
  PpuStatus = 2,
  OamAddr = 3,
  OamData = 4,
  PpuScroll = 5,
  PpuAddr = 6,
  PpuData = 7,
};

enum class MemoryMap {
  Unknown,
  Chr,
  Nametables,
  Palettes,
};

auto get_memory_map(uint16_t) -> MemoryMap;

enum class MirroringType {
  Unknown,
  Horizontal,
  Vertical,
  OneScreenLow,
  OneScreenHigh,
  FourScreen,
};

auto get_mirroring_name(MirroringType type) -> std::string_view;
}  // namespace nes::types::ppu
