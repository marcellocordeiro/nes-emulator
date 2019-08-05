#include "apu.h"

#include "Nes_Apu.h"
#include "Sound_Queue.h"

#include "cpu.h"
#include "emulator.h"

namespace nes {
apu::apu(nes::emulator& emulator_ref)
  : emulator(emulator_ref), nes_apu(std::make_unique<Nes_Apu>()),
    buffer(std::make_unique<Blip_Buffer>()),
    sound_queue(std::make_unique<Sound_Queue>())
{}

apu::~apu() = default;

void apu::power_on()
{
  buffer->sample_rate(44100);
  buffer->clock_rate(1789773);

  nes_apu->output(buffer.get());
  nes_apu->volume(0.1);

  sound_queue->init(44100);

  nes_apu->dmc_reader(
      [](void* user_data, cpu_addr_t addr) -> int {
        auto a_cpu = static_cast<nes::cpu*>(user_data);
        return a_cpu->dmc_reader(addr);
      },
      emulator.get_cpu());
}

uint8_t apu::read(int elapsed)
{
  return static_cast<uint8_t>(nes_apu->read_status(elapsed));
}

void apu::write(int elapsed, uint16_t addr, uint8_t value)
{
  nes_apu->write_register(elapsed, addr, value);
}

void apu::run_frame(int elapsed)
{
  nes_apu->end_frame(elapsed);
  buffer->end_frame(elapsed);

  if (buffer->samples_avail() >= buffer_size) {
    sound_queue->write(
        out_buffer.data(),
        buffer->read_samples(out_buffer.data(), buffer_size));
  }
}
}  // namespace nes
