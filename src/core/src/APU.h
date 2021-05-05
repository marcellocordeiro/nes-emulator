#pragma once

#include <common.h>

namespace nes {
class APU final {
public:
  APU(const APU&) = delete;
  APU(APU&&)      = delete;
  APU& operator=(const APU&) = delete;
  APU& operator=(APU&&) = delete;

  static APU& get();

  void power_on();

  uint8_t read();
  void    write(uint16_t, uint8_t);

private:
  APU() = default;
};
}  // namespace nes
