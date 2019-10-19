#include "APU.h"

#include "CPU.h"

#include <Nes_Apu.h>
Nes_Apu     nes_apu;
Blip_Buffer buffer;

namespace nes {
apu& apu::get()
{
  static apu instance;
  return instance;
}

void apu::power_on()
{
  buffer.sample_rate(44100);
  buffer.clock_rate(1789773);

  nes_apu.output(&buffer);

  nes_apu.dmc_reader(
      [](void* user_data, cpu_addr_t addr) -> int {
        return static_cast<cpu*>(user_data)->peek(addr);
      },
      &cpu::get());
}

void apu::volume(double value) { nes_apu.volume(value); }

uint8_t apu::read(int elapsed)
{
  return static_cast<uint8_t>(nes_apu.read_status(elapsed));
}

void apu::write(int elapsed, uint16_t addr, uint8_t value)
{
  nes_apu.write_register(elapsed, addr, value);
}

void apu::end_frame(int elapsed)
{
  nes_apu.end_frame(elapsed);
  buffer.end_frame(elapsed);
}

bool apu::samples_available(size_t buffer_size) const
{
  return buffer.samples_avail() >= static_cast<long>(buffer_size);
}

long apu::get_samples(int16_t* audio_buffer, size_t size)
{
  return buffer.read_samples(audio_buffer, static_cast<long>(size));
}
}  // namespace nes