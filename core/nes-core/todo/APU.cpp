#include "apu.h"

namespace nes {
auto Apu::get() -> Apu& {
  static Apu instance;
  return instance;
}

void Apu::power_on() {}

auto Apu::read() -> uint8_t { return 0; }

void Apu::write(uint16_t addr, uint8_t value) {
  UNUSED(addr);
  UNUSED(value);
}
}  // namespace nes
