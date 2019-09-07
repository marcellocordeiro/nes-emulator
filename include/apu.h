#pragma once

#include <array>
#include <memory>

#include "common.h"

class Nes_Apu;
class Blip_Buffer;

namespace nes {
class emulator;

class apu {
public:
  apu(emulator&);
  ~apu();  // Has to be defined in the cpp

  void power_on();
  void volume(double);

  uint8_t read(int);
  void    write(int, uint16_t, uint8_t);

  void end_frame(int);
  bool samples_available(size_t) const;
  long get_samples(int16_t*, size_t);

private:
  emulator& emu;

  std::unique_ptr<Nes_Apu>     nes_apu;
  std::unique_ptr<Blip_Buffer> buffer;
};
}  // namespace nes
