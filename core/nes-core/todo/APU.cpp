#include "apu.h"

namespace nes {
auto Apu::get() -> Apu& {
  static Apu instance;
  return instance;
}

void Apu::power_on() {}

auto Apu::read() -> u8 {
  return 0;
}

void Apu::write(u16 addr, u8 value) {
  UNUSED(addr);
  UNUSED(value);
}
} // namespace nes
