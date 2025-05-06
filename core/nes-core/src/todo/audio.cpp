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

void Audio::write(u16 addr, u8 value) {
  UNUSED(addr);
  UNUSED(value);
}
} // namespace nes
