#include "nes/emulator.h"

#include <log.h>
#include <nes/apu.h>
#include <nes/base_mapper.h>
#include <nes/cartridge.h>
#include <nes/controller.h>
#include <nes/cpu.h>
#include <nes/debugger.h>
#include <nes/ppu.h>
#include <nes/utility/file_manager.h>
#include <nes/utility/snapshotable.h>

namespace nes {
emulator::emulator()
  : cpu_ptr(std::make_unique<cpu>(*this)),
    ppu_ptr(std::make_unique<ppu>(*this)),
    apu_ptr(std::make_unique<apu>(*this)),
    cartridge_ptr(std::make_unique<cartridge>(*this)),
    controller_ptr(std::make_unique<controller>()),
    debugger_ptr(std::make_unique<debugger>(*this))
{}

emulator::~emulator() = default;

void emulator::power_on()
{
  cartridge_ptr->load();

  cpu_ptr->power_on();
  ppu_ptr->power_on();
  apu_ptr->power_on();

  snapshotable.push_back(cpu_ptr.get());
  snapshotable.push_back(ppu_ptr.get());
  snapshotable.push_back(cartridge_ptr->get_mapper());
  snapshotable.push_back(controller_ptr.get());
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

debugger* emulator::get_debugger()
{
  return debugger_ptr.get();
}

//
// Snapshot
//

void emulator::save_snapshot()
{
  std::ofstream out{util::fmngr.get_snapshot_path(), std::ios::binary};
  for (auto& component : snapshotable) component->save(out);
}

void emulator::load_snapshot()
{
  if (!util::fmngr.has_snapshot()) {
    LOG(Info, "Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{util::fmngr.get_snapshot_path(), std::ios::binary};
  for (auto& component : snapshotable) component->load(in);
}
}  // namespace nes
