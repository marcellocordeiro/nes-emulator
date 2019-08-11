#pragma once

#include <array>
#include <memory>

#include "common.h"
#include "types/forward_decl.h"

namespace nes {
class apu {
public:
  apu(nes::emulator&);
  ~apu();  // Has to be defined in the cpp

  void power_on();
  void volume(double);

  uint8_t read(int);
  void    write(int, uint16_t, uint8_t);

  void run_frame(int);

private:
  nes::emulator& emulator;

  static constexpr int buffer_size = 4096;

  std::unique_ptr<Nes_Apu>     nes_apu;
  std::unique_ptr<Blip_Buffer> buffer;
  std::unique_ptr<Sound_Queue> sound_queue;

  std::array<int16_t, buffer_size> out_buffer{};
};
}  // namespace nes
