#pragma once

#include <common.h>

namespace nes {
class APU final {
public:
  APU(const APU&) = delete;
  APU(APU&&) = delete;
  auto operator=(const APU&) -> APU& = delete;
  auto operator=(APU&&) -> APU& = delete;

  static auto get() -> APU&;

  void power_on();

  auto read() -> uint8_t;
  void write(uint16_t, uint8_t);

private:
  APU() = default;
};
}  // namespace nes
