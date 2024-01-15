#include "PPU_Types.h"

namespace nes::types::ppu {
auto get_memory_map(uint16_t addr) -> memory_map {
  auto in_range = [addr](int lower, int upper) {
    return (addr >= lower) && (addr <= upper);
  };

  if (in_range(0x0000, 0x1FFF)) {
    return memory_map::CHR;
  }

  if (in_range(0x2000, 0x3EFF)) {
    return memory_map::Nametables;
  }

  if (in_range(0x3F00, 0x3FFF)) {
    return memory_map::Palettes;
  }

  return memory_map::Unknown;
}

auto get_mirroring_name(mirroring_type type) -> std::string_view {
  static std::string_view horizontal = "Horizontal";
  static std::string_view vertical = "Vertical";
  static std::string_view one_screen_low = "One Screen Low";
  static std::string_view one_screen_high = "One Screen High";
  static std::string_view four_screen = "Four Screen";
  static std::string_view unknown = "Unknown";

  switch (type) {
    case mirroring_type::Horizontal: return horizontal;
    case mirroring_type::Vertical: return vertical;
    case mirroring_type::One_Screen_Low: return one_screen_low;
    case mirroring_type::One_Screen_High: return one_screen_high;
    case mirroring_type::Four_Screen: return four_screen;
    default: return unknown;
  }
}
}  // namespace nes::types::ppu
