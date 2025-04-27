#pragma once

#include <lib/common.h>

namespace nes {
class Apu final {
public:
  Apu(const Apu&) = delete;
  Apu(Apu&&) = delete;
  auto operator=(const Apu&) -> Apu& = delete;
  auto operator=(Apu&&) -> Apu& = delete;

  static auto get() -> Apu&;

  void power_on();

  auto read() -> u8;
  void write(u16, u8);

private:
  Apu() = default;
};
} // namespace nes
