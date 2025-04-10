#pragma once

#include <common.h>

namespace nes {
class Apu final {
public:
  Apu(const Apu&) = delete;
  Apu(Apu&&) = delete;
  auto operator=(const Apu&) -> Apu& = delete;
  auto operator=(Apu&&) -> Apu& = delete;

  static auto get() -> Apu&;

  void power_on();

  auto read() -> uint8_t;
  void write(uint16_t, uint8_t);

private:
  Apu() = default;
};
} // namespace nes
