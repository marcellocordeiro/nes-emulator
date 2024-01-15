#include "nes/Emulator.h"

#include <spdlog/spdlog.h>

#include "Cartridge.h"
#include "Controller.h"
#include "Utility/FileManager.h"
#include "cpu.h"
#include "ppu.h"

using namespace nes;

void Emulator::set_app_path(const std::filesystem::path& path) {
  Utility::FileManager::get().set_app_path(path);
}

void Emulator::load(const std::filesystem::path& path) {
  Utility::FileManager::get().set_rom(path);
}

void Emulator::reset() {
  Cpu::get().reset();
  Ppu::get().reset();
}

void Emulator::power_on() {
  auto irq = std::make_shared<bool>(false);
  Cartridge::get().irq_conn = irq;
  Cpu::get().irq_conn = irq;

  auto nmi = std::make_shared<bool>(false);
  Cpu::get().nmi_conn = nmi;
  Ppu::get().nmi_conn = nmi;

  auto mirroring = std::make_shared<types::ppu::MirroringType>(
    types::ppu::MirroringType::Unknown
  );
  Ppu::get().mirroring_conn = mirroring;
  Cartridge::get().mirroring_conn = mirroring;

  Cartridge::get().load();

  Cpu::get().power_on();
  Ppu::get().power_on();
}

void Emulator::power_off() { Cartridge::get().dump_prg_ram(); }

void Emulator::run_frame() { Cpu::get().run_frame(); }

auto Emulator::get_back_buffer() -> const uint32_t* {
  return Ppu::get().get_back_buffer();
}

void Emulator::update_controller_state(size_t port, uint8_t state) {
  nes::Controller::get().update_state(port, state);
}

//
// Snapshot
//

void Emulator::save_snapshot() {
  std::array<Utility::Snapshotable*, 4> snapshotable = {
    &Cpu::get(), &Ppu::get(), Cartridge::get().get_mapper(), &Controller::get()
  };

  std::ofstream out{
    Utility::FileManager::get().get_snapshot_path(), std::ios::binary
  };
  for (const auto& component : snapshotable) component->save(out);
}

void Emulator::load_snapshot() {
  std::array<Utility::Snapshotable*, 4> snapshotable = {
    &Cpu::get(), &Ppu::get(), Cartridge::get().get_mapper(), &Controller::get()
  };

  if (!Utility::FileManager::get().has_snapshot()) {
    spdlog::info("Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{
    Utility::FileManager::get().get_snapshot_path(), std::ios::binary
  };
  for (auto& component : snapshotable) component->load(in);
}
