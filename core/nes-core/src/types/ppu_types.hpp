#pragma once

#include <string_view>

#include "lib/bitfield.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
template <usize Position, usize Bits>
using BitfieldU8 = lib::Bitfield<u8, Position, Bits>;

template <usize Position, usize Bits>
using BitfieldU16 = lib::Bitfield<u16, Position, Bits>;

// PPUCTRL ($2000) register
union ppuctrl {
  u8 raw = 0;

  BitfieldU8<0, 2> nt;        // Base nametable addr [$2000, $2400, $2800, $2C00]
  BitfieldU8<2, 1> addr_inc;  // Address increment [1, 32]
  BitfieldU8<3, 1> spr_table; // Sprite pattern table [$0000, $1000]
  BitfieldU8<4, 1> bg_table;  // Background pattern table [$0000, $1000]
  BitfieldU8<5, 1> spr_size;  // Sprite size [8x8, 8x16]
  BitfieldU8<7, 1> nmi;       // Enable NMI on the next VBlank

  // BitfieldU8<6, 1> slave;  // PPU master/slave (unused)
};

// PPUMASK ($2001) register
union ppumask {
  u8 raw = 0;

  BitfieldU8<0, 1> grayscale; // Grayscale
  BitfieldU8<1, 1> bg_left;   // Show background in the first column (8 pixels)
  BitfieldU8<2, 1> spr_left;  // Show sprite in the first column (8 pixels)
  BitfieldU8<3, 1> show_bg;   // Show background
  BitfieldU8<4, 1> show_spr;  // Show sprites
  BitfieldU8<5, 3> rgb;       // Palette picker (colour emphasis)
};

// PPUSTATUS ($2002) register
union ppustatus {
  u8 raw = 0;

  BitfieldU8<5, 1> spr_overflow; // Sprite overflow
  BitfieldU8<6, 1> spr0_hit;     // Sprite 0 hit
  BitfieldU8<7, 1> vblank;       // In VBlank?
};

// Loopy's VRAM address
union LoopyAddr {
  u16 raw = 0;

  BitfieldU16<0, 5> coarse_x; // Coarse X
  BitfieldU16<5, 5> coarse_y; // Coarse Y
  BitfieldU16<10, 2> nt;      // Nametable
  BitfieldU16<12, 3> fine_y;  // Fine Y

  BitfieldU16<0, 8> l; // Low addr part
  BitfieldU16<8, 7> h; // High addr part

  BitfieldU16<0, 14> addr;
};

// Sprite info
struct SpriteInfo {
  u8 y = 64;      // Y position
  u8 tile = 0xFF; // Tile index
  u8 attr = 0xFF; // Attributes
  u8 x = 0xFF;    // X position

  usize id = 0xFF; // Index in OAM
  u8 data_l = 0;   // Tile data (low)
  u8 data_h = 0;   // Tile data (high)
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

auto get_memory_map(u16) -> MemoryMap;

enum class MirroringType {
  Unknown,
  Horizontal,
  Vertical,
  OneScreenLow,
  OneScreenHigh,
  FourScreen,
};

auto get_mirroring_name(MirroringType type) -> std::string_view;
} // namespace nes::types::ppu
