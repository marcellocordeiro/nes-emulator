#pragma once

#include <string_view>

#include "lib/common.hpp"
#include "ppu/loopy_addr.hpp" // IWYU pragma: export
#include "ppu/ppuctrl.hpp"    // IWYU pragma: export
#include "ppu/ppumask.hpp"    // IWYU pragma: export
#include "ppu/ppustatus.hpp"  // IWYU pragma: export

namespace nes::types::ppu {
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
