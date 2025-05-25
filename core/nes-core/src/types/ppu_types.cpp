#include "ppu_types.hpp"

#include <string_view>

#include "lib/common.hpp"

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
  constexpr static std::string_view HORIZONTAL = "Horizontal";
  constexpr static std::string_view VERTICAL = "Vertical";
  constexpr static std::string_view ONE_SCREEN_LOW = "One Screen Low";
  constexpr static std::string_view ONE_SCREEN_HIGH = "One Screen High";
  constexpr static std::string_view FOUR_SCREEN = "Four Screen";
  constexpr static std::string_view UNKNOWN = "Unknown";

  switch (type) {
    case MirroringType::Horizontal: return HORIZONTAL;
    case MirroringType::Vertical: return VERTICAL;
    case MirroringType::OneScreenLow: return ONE_SCREEN_LOW;
    case MirroringType::OneScreenHigh: return ONE_SCREEN_HIGH;
    case MirroringType::FourScreen: return FOUR_SCREEN;
    case MirroringType::Unknown: return UNKNOWN;

    default: unreachable();
  }
}
} // namespace nes::types::ppu
