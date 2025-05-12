#include "ppu_types.hpp"

namespace nes::types::ppu {
auto get_memory_map(u16 addr) -> MemoryMap {
  auto in_range = [addr](const u16 lower, const u16 upper) {
    return (addr >= lower) && (addr <= upper);
  };

  if (in_range(0x0000, 0x1FFF)) {
    return MemoryMap::Chr;
  }

  if (in_range(0x2000, 0x3EFF)) {
    return MemoryMap::Nametables;
  }

  if (in_range(0x3F00, 0x3FFF)) {
    return MemoryMap::Palettes;
  }

  return MemoryMap::Unknown;
}

auto get_mirroring_name(const MirroringType type) -> std::string_view {
  constexpr static std::string_view horizontal = "Horizontal";
  constexpr static std::string_view vertical = "Vertical";
  constexpr static std::string_view one_screen_low = "One Screen Low";
  constexpr static std::string_view one_screen_high = "One Screen High";
  constexpr static std::string_view four_screen = "Four Screen";
  constexpr static std::string_view unknown = "Unknown";

  switch (type) {
  case MirroringType::Horizontal: return horizontal;
  case MirroringType::Vertical: return vertical;
  case MirroringType::OneScreenLow: return one_screen_low;
  case MirroringType::OneScreenHigh: return one_screen_high;
  case MirroringType::FourScreen: return four_screen;
  default: return unknown;
  }
}
} // namespace nes::types::ppu
