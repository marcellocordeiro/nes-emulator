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
  void volume(double);

  uint8_t read(int);
  void    write(int, uint16_t, uint8_t);

  void end_frame(int);
  bool samples_available(size_t) const;
  long get_samples(int16_t*, size_t);

private:
  APU() = default;
};
}  // namespace nes
