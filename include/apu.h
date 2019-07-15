#pragma once

#include <array>
#include <memory>

#include "bus.h"
#include "common.h"

#include "Nes_Apu.h"
#include "Sound_Queue.h"

namespace nes {
class apu {
public:
  void set_bus(nes::bus&);

  void power_on();

  uint8_t read(int);
  void    write(int, uint16_t, uint8_t);

  void run_frame(int);

private:
  nes::bus* bus = nullptr;

  static constexpr int buffer_size = 4096;

  std::unique_ptr<Nes_Apu>     nes_apu{new Nes_Apu};
  std::unique_ptr<Blip_Buffer> buffer{new Blip_Buffer};
  std::unique_ptr<Sound_Queue> sound_queue{new Sound_Queue};

  std::array<blip_sample_t, buffer_size> out_buffer;
};
}  // namespace nes
