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
  cartridge::get().load();

  cpu::get().power_on();
  ppu::get().power_on();
  apu::get().power_on();

  snapshotable.push_back(&cpu::get());
  snapshotable.push_back(&ppu::get());
  snapshotable.push_back(cartridge::get().get_mapper());
  snapshotable.push_back(&controller::get());
}

//
// Snapshot
//

void Emulator::save_snapshot()
{
  std::ofstream out{util::file_manager::get().get_snapshot_path(),
                    std::ios::binary};
  for (auto& component : snapshotable) component->save(out);
}

void Emulator::load_snapshot()
{
  if (!util::file_manager::get().has_snapshot()) {
    spdlog::info("Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{util::file_manager::get().get_snapshot_path(),
                   std::ios::binary};
  for (auto& component : snapshotable) component->load(in);
}
}  // namespace nes
