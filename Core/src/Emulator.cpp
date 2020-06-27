#include "nes/Emulator.h"

#include <spdlog/spdlog.h>

#include "APU.h"
#include "BaseMapper.h"
#include "CPU.h"
#include "Cartridge.h"
#include "Controller.h"
#include "PPU.h"
#include "Utility/FileManager.h"

using namespace nes;

void Emulator::set_app_path(const std::filesystem::path& path) { Utility::FileManager::get().set_app_path(path); }

void Emulator::load(const std::filesystem::path& path) { Utility::FileManager::get().set_rom(path); }

void Emulator::reset()
{
  CPU::get().reset();
  PPU::get().reset();
}

void Emulator::power_on()
{
  Cartridge::get().load();

  CPU::get().power_on();
  PPU::get().power_on();
  APU::get().power_on();
}

void Emulator::power_off() { Cartridge::get().dump_prg_ram(); }

void Emulator::run_frame() { CPU::get().run_frame(); }

const uint32_t* Emulator::get_back_buffer() { return PPU::get().get_back_buffer(); }

void Emulator::update_controller_state(size_t port, uint8_t state) { nes::Controller::get().update_state(port, state); }

//
// Snapshot
//

void Emulator::save_snapshot()
{
  std::array<Utility::Snapshotable*, 4> snapshotable = {&CPU::get(), &PPU::get(), Cartridge::get().get_mapper(),
                                                        &Controller::get()};

  std::ofstream out{Utility::FileManager::get().get_snapshot_path(), std::ios::binary};
  for (auto& component : snapshotable) component->save(out);
}

void Emulator::load_snapshot()
{
  std::array<Utility::Snapshotable*, 4> snapshotable = {&CPU::get(), &PPU::get(), Cartridge::get().get_mapper(),
                                                        &Controller::get()};

  if (!Utility::FileManager::get().has_snapshot()) {
    spdlog::info("Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{Utility::FileManager::get().get_snapshot_path(), std::ios::binary};
  for (auto& component : snapshotable) component->load(in);
}