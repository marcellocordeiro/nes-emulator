#include "APU.h"

namespace nes {
auto APU::get() -> APU&
{
  static APU instance;
  return instance;
}

void APU::power_on() {}

auto APU::read() -> uint8_t { return 0; }

void APU::write(uint16_t addr, uint8_t value)
{
  (void)addr;
  (void)value;
}
}  // namespace nes
