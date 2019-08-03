#include "emulator.h"

#include "common.h"

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "io.h"
#include "log.h"
#include "mapper.h"
#include "ppu.h"
#include "utility/file_manager.h"
#include "utility/snapshotable.h"

namespace nes {
emulator::emulator()
  : cpu{std::make_unique<nes::cpu>(*this)},
    ppu{std::make_unique<nes::ppu>(*this)}, apu{std::make_unique<nes::apu>(
                                                *this)},
    cartridge{std::make_unique<nes::cartridge>(*this)},
    controller{std::make_unique<nes::controller>(*this)},
    io{std::make_unique<nes::io>(*this)}
{}

emulator::~emulator() = default;

void emulator::load_rom(const char* path)
{
  cartridge->load(path);
}

void emulator::power_on()
{
  cpu->power_on();
  ppu->power_on();
  apu->power_on();
}

void emulator::run()
{
  snapshotable.push_back(cpu.get());
  snapshotable.push_back(ppu.get());
  snapshotable.push_back(cartridge->get_mapper());

  io->run();
}

nes::cpu* emulator::get_cpu()
{
  return cpu.get();
}

nes::ppu* emulator::get_ppu()
{
  return ppu.get();
}

nes::apu* emulator::get_apu()
{
  return apu.get();
}

nes::cartridge* emulator::get_cartridge()
{
  return cartridge.get();
}

nes::controller* emulator::get_controller()
{
  return controller.get();
}

nes::io* emulator::get_io()
{
  return io.get();
}

//
// Snapshot
//

void emulator::save_snapshot()
{
  std::ofstream out{util::fmngr.get_snapshot(), std::ios::binary};
  for (auto& component : snapshotable) component->save(out);
}

void emulator::load_snapshot()
{
  std::ifstream in{util::fmngr.get_snapshot(), std::ios::binary};
  for (auto& component : snapshotable) component->load(in);
}
}  // namespace nes
