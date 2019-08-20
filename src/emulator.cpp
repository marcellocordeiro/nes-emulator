#include "emulator.h"

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "debugger.h"
#include "io.h"
#include "mapper.h"
#include "ppu.h"
#include "utility/file_manager.h"
#include "utility/snapshotable.h"

namespace nes {
emulator::emulator()
  : cpu_ptr(std::make_unique<cpu>(*this)),
    ppu_ptr(std::make_unique<ppu>(*this)),
    apu_ptr(std::make_unique<apu>(*this)),
    cartridge_ptr(std::make_unique<cartridge>(*this)),
    controller_ptr(std::make_unique<controller>(*this)),
    io_ptr(std::make_unique<io>(*this)),
    debugger_ptr(std::make_unique<debugger>(*this))
{}

emulator::~emulator() = default;

void emulator::load_rom(const char* path)
{
  cartridge_ptr->load(path);
}

void emulator::power_on()
{
  cpu_ptr->power_on();
  ppu_ptr->power_on();
  apu_ptr->power_on();

  io_ptr->start();
}

void emulator::run()
{
  snapshotable.push_back(cpu_ptr.get());
  snapshotable.push_back(ppu_ptr.get());
  snapshotable.push_back(cartridge_ptr->get_mapper());
  snapshotable.push_back(controller_ptr.get());

  io_ptr->run();
}

cpu* emulator::get_cpu()
{
  return cpu_ptr.get();
}

ppu* emulator::get_ppu()
{
  return ppu_ptr.get();
}

apu* emulator::get_apu()
{
  return apu_ptr.get();
}

cartridge* emulator::get_cartridge()
{
  return cartridge_ptr.get();
}

controller* emulator::get_controller()
{
  return controller_ptr.get();
}

io* emulator::get_io()
{
  return io_ptr.get();
}

debugger* emulator::get_debugger()
{
  return debugger_ptr.get();
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
