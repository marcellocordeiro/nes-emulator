#include "APU.h"

namespace nes {
APU& APU::get()
{
  static APU instance;
  return instance;
}

void APU::power_on() {}

uint8_t APU::read() { return 0; }

void APU::write(uint16_t addr, uint8_t value)
{
  (void)addr;
  (void)value;
}
}  // namespace nes
