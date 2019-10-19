#pragma once

#include <common.h>

namespace nes {
class apu {
public:
  apu(const apu&) = delete;
  apu(apu&&)      = delete;
  apu& operator=(const apu&) = delete;
  apu& operator=(apu&&) = delete;

  static apu& get();

  void power_on();
  void volume(double);

  uint8_t read(int);
  void    write(int, uint16_t, uint8_t);

  void end_frame(int);
  bool samples_available(size_t) const;
  long get_samples(int16_t*, size_t);

private:
  apu() = default;
};
}  // namespace nes
