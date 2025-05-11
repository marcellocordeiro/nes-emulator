#pragma once

#include "lib/common.hpp"

namespace nes {
class Audio final {
public:
  static auto get() -> Audio&;

  void power_on();

  auto read() -> u8;
  void write(u16 addr, u8 value);

private:
  Audio() = default;
};
} // namespace nes
