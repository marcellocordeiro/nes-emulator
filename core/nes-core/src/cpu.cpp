#include "cpu.hpp"

#include <iosfwd>
#include <stdexcept>

#include "spdlog/spdlog.h"

#include "cartridge.hpp"
#include "controller.hpp"
#include "lib/common.hpp"
#include "ppu.hpp"
#include "types/cpu_types.hpp"

namespace nes {
auto Cpu::get() -> Cpu& {
  static Cpu instance;
  return instance;
}

void Cpu::power_on() {
  state.a = 0;
  state.x = 0;
  state.y = 0;
  state.pc = static_cast<u16>(peek(0xFFFC + 1) << 8) | peek(0xFFFC);
  state.sp = 0xFD;

  state.cycle_count = 0;
  state.set_ps(0x34);
  ram.fill(0);

  // nestest
  // state.pc          = 0xC000;
  // state.cycle_count = 7;
}

void Cpu::reset() {
  INT_RST();
}

void Cpu::dma_oam(const u16 addr) {
  for (u16 i = 0; i < 256; ++i) {
    // 0x2004 == OAMDATA
    memory_write(0x2004, memory_read((addr * 0x100) + i));
  }
}

void Cpu::run_frame() {
  using types::cpu::memory::get_map;
  using enum types::cpu::flags;

  constexpr auto cycles_per_frame = 29781;

  state.cycle_count %= cycles_per_frame;

  while (state.cycle_count < cycles_per_frame) {
    if (*nmi_conn) {
      INT_NMI();
    } else if (*irq_conn && !state.check_flags(Interrupt)) {
      INT_IRQ();
    }

    execute();
  }
}

void Cpu::tick() {
  Ppu::get().step();
  Ppu::get().step();
  Ppu::get().step();
  ++state.cycle_count;
}

auto Cpu::peek(u16 addr) const -> u8 {
  using types::cpu::memory::get_map;
  using enum types::cpu::memory::MemoryMap;
  using enum types::cpu::memory::Operation;

  switch (get_map<Read>(addr)) {
  case CpuRam: return ram[addr & 0x07FF];
  case PpuAccess: return Ppu::get().peek_reg(addr);
  case ApuAccess: return 0xFF; // Avoids APU side effects and satisfies nestest
  case Controller1: return Controller::get().peek(0);
  case Controller2: return Controller::get().peek(1);
  case CartridgeAccess: return Cartridge::get().prg_read(addr);

  case OamDma:
  case ControllerAccess:
  case Unknown: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;

  default: unreachable();
  }
}

auto Cpu::read(u16 addr) const -> u8 {
  using types::cpu::memory::get_map;
  using enum types::cpu::memory::MemoryMap;
  using enum types::cpu::memory::Operation;

  switch (get_map<Read>(addr)) {
  case CpuRam: return ram[addr & 0x07FF];
  case PpuAccess: return Ppu::get().read(addr);
  case ApuAccess: return 0;
  case Controller1: return Controller::get().read(0);
  case Controller2: return Controller::get().read(1);
  case CartridgeAccess: return Cartridge::get().prg_read(addr);

  case OamDma:
  case ControllerAccess:
  case Unknown: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;

  default: unreachable();
  }
}

void Cpu::write(const u16 addr, const u8 value) {
  using types::cpu::memory::get_map;
  using enum types::cpu::memory::MemoryMap;
  using enum types::cpu::memory::Operation;

  switch (get_map<Write>(addr)) {
  case CpuRam: ram[addr & 0x07FF] = value; break;
  case PpuAccess: Ppu::get().write(addr, value); break;
  case ApuAccess: break;
  case OamDma: dma_oam(value); break;
  case ControllerAccess: Controller::get().write((value & 1) != 0); break;
  case CartridgeAccess: Cartridge::get().prg_write(addr, value); break;

  case Controller1:
  case Controller2:
  case Unknown: throw std::runtime_error("Invalid write address");

  default: unreachable();
  }
}

auto Cpu::memory_read(const u16 addr) -> u8 {
  tick();
  return read(addr);
}

void Cpu::memory_write(const u16 addr, const u8 value) {
  tick();
  write(addr, value);
}

auto Cpu::get_state() const -> types::cpu::State {
  return state;
}

auto Cpu::peek_imm() const -> u16 {
  return state.pc + 1;
}

auto Cpu::peek_rel() const -> u16 {
  const auto addr = peek_imm();
  const auto offset = static_cast<i16>(static_cast<i8>(peek(addr)));
  const auto base = state.pc + 2;

  return static_cast<u16>(static_cast<i16>(base) + offset);
}

auto Cpu::peek_zp() const -> u16 {
  return peek(peek_imm());
}

auto Cpu::peek_zpx() const -> u16 {
  return (peek_zp() + state.x) & 0xFF;
}

auto Cpu::peek_zpy() const -> u16 {
  return (peek_zp() + state.y) & 0xFF;
}

auto Cpu::peek_ab() const -> u16 {
  const auto base_addr = peek_imm();

  const auto low = peek(base_addr);
  const auto high = peek(base_addr + 1);

  return static_cast<u8>(high << 8) | low;
}

auto Cpu::peek_abx() const -> u16 {
  const auto base_addr = peek_ab();
  const auto offset = state.x;

  return base_addr + offset;
}

auto Cpu::peek_aby() const -> u16 {
  const auto base_addr = peek_ab();
  const auto offset = state.y;

  return base_addr + offset;
}

auto Cpu::peek_ind() const -> u16 {
  const auto base_addr = peek_ab();

  const auto low = peek(base_addr);
  const auto high = peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100));

  return static_cast<u16>(high << 8) | low;
}

auto Cpu::peek_indx() const -> u16 {
  const auto base_addr = peek_zpx();

  const auto low = peek(base_addr);
  const auto high = peek((base_addr + 1) & 0xFF);

  return static_cast<u16>(high << 8) | low;
}

auto Cpu::peek_indy() const -> u16 {
  const auto base_addr = peek_zp();

  const auto low = peek(base_addr);
  const auto high = peek((base_addr + 1) & 0xFF);

  const auto offset = state.y;

  return static_cast<u16>(static_cast<u16>(high << 8) | low) + offset;
}
} // namespace nes
