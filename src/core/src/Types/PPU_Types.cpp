#include "PPU_Types.h"

namespace nes::types::ppu {
auto get_memory_map(uint16_t addr) -> int
{
  auto in_range = [addr](int lower, int upper) { return (addr >= lower) && (addr <= upper); };

  if (in_range(0x0000, 0x1FFF)) {
    return CHR;
  }

  if (in_range(0x2000, 0x3EFF)) {
    return Nametables;
  }

  if (in_range(0x3F00, 0x3FFF)) {
    return Palettes;
  }

  return Unknown;
}
}  // namespace nes::types::ppu
