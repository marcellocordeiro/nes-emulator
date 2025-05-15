#include "audio.hpp"

namespace nes {
auto Audio::get() -> Audio& {
  static Audio instance;
  return instance;
}

void Audio::power_on() {}

auto Audio::read() -> u8 {
  return 0;
}

void Audio::write(const u16 addr, const u8 value) {
  UNUSED(addr);
  UNUSED(value);
}
} // namespace nes
