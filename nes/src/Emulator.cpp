#include "nes/Emulator.h"

#include <spdlog/spdlog.h>

namespace nes {
Emulator& Emulator::get()
{
  static Emulator instance;
  return instance;
}

void Emulator::power_on()
{
  Cartridge::get().load();

  CPU::get().power_on();
  PPU::get().power_on();
  APU::get().power_on();

  snapshotable.push_back(&CPU::get());
  snapshotable.push_back(&PPU::get());
  snapshotable.push_back(Cartridge::get().get_mapper());
  snapshotable.push_back(&Controller::get());
}

void Emulator::reset()
{
  CPU::get().reset();
  PPU::get().reset();
}

void Emulator::volume(double value) { APU::get().volume(value); }

//
// Snapshot
//

void Emulator::save_snapshot()
{
  std::ofstream out{Utility::FileManager::get().get_snapshot_path(),
                    std::ios::binary};
  for (auto& component : snapshotable) component->save(out);
}

void Emulator::load_snapshot()
{
  if (!Utility::FileManager::get().has_snapshot()) {
    spdlog::info("Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{Utility::FileManager::get().get_snapshot_path(),
                   std::ios::binary};
  for (auto& component : snapshotable) component->load(in);
}
}  // namespace nes
