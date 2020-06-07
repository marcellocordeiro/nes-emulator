#include "PPU_Types.h"

namespace nes::types::ppu::memory {
int get_mem_map(uint16_t addr)
{
  auto in_range = [addr](int lower, int upper) {
    return (addr >= lower) && (addr <= upper);
  };

  if (in_range(0x0000, 0x1FFF)) {
    return CHR;
  } else if (in_range(0x2000, 0x3EFF)) {
    return Nametables;
  } else if (in_range(0x3F00, 0x3FFF)) {
    return Palettes;
  } else {
    return Unknown;
  }
}
}  // namespace nes::types::ppu::memory
