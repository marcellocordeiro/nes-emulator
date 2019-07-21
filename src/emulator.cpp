#include "emulator.h"

namespace nes {
emulator::emulator()
  : cpu{ppu, apu, cartridge, controller}, ppu{cpu, cartridge, io}, apu{cpu},
    cartridge{cpu, ppu}, controller{io}, io{cpu, cartridge}
{}

void emulator::load_rom(const char* path)
{
  cartridge.load(path);
}

void emulator::power_on()
{
  cpu.power_on();
  ppu.power_on();
  apu.power_on();
}

void emulator::run()
{
  io.run();
}
}  // namespace nes
