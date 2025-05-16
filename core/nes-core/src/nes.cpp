#include "nes/nes.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>

#include <spdlog/spdlog.h>

#include "cartridge.hpp"
#include "controller.hpp"
#include "cpu.hpp"
#include "lib/common.hpp"
#include "ppu.hpp"
#include "types/ppu_types.hpp"
#include "utility/file_manager.hpp"
#include "utility/snapshotable.hpp"

namespace nes {
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
  const auto irq = std::make_shared<bool>(false);
  Cartridge::get().irq_conn = irq;
  Cpu::get().irq_conn = irq;

  const auto nmi = std::make_shared<bool>(false);
  Cpu::get().nmi_conn = nmi;
  Ppu::get().nmi_conn = nmi;

  const auto mirroring =
    std::make_shared<types::ppu::MirroringType>(types::ppu::MirroringType::Unknown);
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

auto Nes::get_frame_buffer() -> const u32* {
  return Ppu::get().get_frame_buffer();
}

void Nes::update_controller_state(const usize port, const u8 state) {
  Controller::get().update_state(port, state);
}
} // namespace nes
