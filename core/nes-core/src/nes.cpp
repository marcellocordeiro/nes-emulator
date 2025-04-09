#include "nes/nes.h"

#include <spdlog/spdlog.h>

#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "ppu.h"
#include "utility/file_manager.h"

using namespace nes;

void Nes::set_app_path(const std::filesystem::path& path) {
  utility::FileManager::get().set_app_path(path);
}

void Nes::load(const std::filesystem::path& path) {
  utility::FileManager::get().set_rom(path);
}

void Nes::reset() {
  Cpu::get().reset();
  Ppu::get().reset();
}

void Nes::power_on() {
  auto irq = std::make_shared<bool>(false);
  Cartridge::get().irq_conn = irq;
  Cpu::get().irq_conn = irq;

  auto nmi = std::make_shared<bool>(false);
  Cpu::get().nmi_conn = nmi;
  Ppu::get().nmi_conn = nmi;

  auto mirroring = std::make_shared<types::ppu::MirroringType>(types::ppu::MirroringType::Unknown);
  Ppu::get().mirroring_conn = mirroring;
  Cartridge::get().mirroring_conn = mirroring;

  Cartridge::get().load();

  Cpu::get().power_on();
  Ppu::get().power_on();
}

void Nes::power_off() {
  Cartridge::get().dump_prg_ram();
}

void Nes::run_frame() {
  Cpu::get().run_frame();
}

auto Nes::get_back_buffer() -> const uint32_t* {
  return Ppu::get().get_back_buffer();
}

void Nes::update_controller_state(size_t port, uint8_t state) {
  nes::Controller::get().update_state(port, state);
}

//
// Snapshot
//

void Nes::save_snapshot() {
  std::array<utility::Snapshotable*, 4> snapshotable =
    {&Cpu::get(), &Ppu::get(), Cartridge::get().get_mapper(), &Controller::get()};

  std::ofstream out{utility::FileManager::get().get_snapshot_path(), std::ios::binary};
  for (const auto& component : snapshotable) {
    component->save(out);
  }
}

void Nes::load_snapshot() {
  std::array<utility::Snapshotable*, 4> snapshotable =
    {&Cpu::get(), &Ppu::get(), Cartridge::get().get_mapper(), &Controller::get()};

  if (!utility::FileManager::get().has_snapshot()) {
    spdlog::info("Attempting to load a non-existent snapshot file");
    return;
  }

  std::ifstream in{utility::FileManager::get().get_snapshot_path(), std::ios::binary};
  for (auto& component : snapshotable) {
    component->load(in);
  }
}
