#include "cpu.h"

#include <spdlog/spdlog.h>

#include "cartridge.h"
#include "controller.h"
#include "ppu.h"

namespace nes {
auto Cpu::get() -> Cpu& {
  static Cpu instance;
  return instance;
}

void Cpu::power_on() {
  state.a = 0;
  state.x = 0;
  state.y = 0;
  state.pc = (peek(0xFFFC + 1) << 8) | peek(0xFFFC);
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

void Cpu::dma_oam(uint8_t addr) {
  for (uint16_t i = 0; i < 256; ++i) {
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

auto Cpu::peek(uint16_t addr) const -> uint8_t {
  using types::cpu::memory::get_map;
  using enum types::cpu::memory::MemoryMap;
  using enum types::cpu::memory::Operation;

  switch (get_map<Read>(addr)) {
  case CpuRam: return ram[addr & 0x07FF];
  case PpuAccess: return Ppu::get().peek_reg(addr);
  case ApuAccess: return 0xFF;  // Avoids APU side effects and satisfies nestest
  case Controller1: return Controller::get().peek(0);
  case Controller2: return Controller::get().peek(1);
  case CartridgeAccess: return Cartridge::get().prg_read(addr);
  case Unknown:
  default: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;
  }
}

auto Cpu::read(uint16_t addr) const -> uint8_t {
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
  case Unknown:
  default: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;
  }
}

void Cpu::write(uint16_t addr, uint8_t value) {
  using types::cpu::memory::get_map;
  using enum types::cpu::memory::MemoryMap;
  using enum types::cpu::memory::Operation;

  switch (get_map<Write>(addr)) {
  case CpuRam: ram[addr & 0x07FF] = value; break;
  case PpuAccess: Ppu::get().write(addr, value); break;
  case ApuAccess: break;
  case OamDma: dma_oam(value); break;
  case ControllerAccess: Controller::get().write(value & 1); break;
  case CartridgeAccess: Cartridge::get().prg_write(addr, value); break;
  default: throw std::runtime_error("Invalid write address");
  }
}

auto Cpu::memory_read(uint16_t addr) -> uint8_t {
  tick();
  return read(addr);
}

void Cpu::memory_write(uint16_t addr, uint8_t value) {
  tick();
  write(addr, value);
}

auto Cpu::get_state() const -> types::cpu::State {
  return state;
}

auto Cpu::peek_imm() const -> uint16_t {
  return state.pc + 1;
}

auto Cpu::peek_rel() const -> uint16_t {
  auto addr = peek_imm();
  auto offset = static_cast<int8_t>(peek(addr));

  return (state.pc + 2) + offset;
}

auto Cpu::peek_zp() const -> uint16_t {
  return peek(peek_imm());
}

auto Cpu::peek_zpx() const -> uint16_t {
  return (peek_zp() + state.x) & 0xFF;
}

auto Cpu::peek_zpy() const -> uint16_t {
  return (peek_zp() + state.y) & 0xFF;
}

auto Cpu::peek_ab() const -> uint16_t {
  const auto base_addr = peek_imm();
  return (peek(base_addr + 1) << 8) | peek(base_addr);
}

auto Cpu::peek_abx() const -> uint16_t {
  const auto base_addr = peek_ab();
  return base_addr + state.x;
}

auto Cpu::peek_aby() const -> uint16_t {
  const auto base_addr = peek_ab();
  return base_addr + state.y;
}

auto Cpu::peek_ind() const -> uint16_t {
  const auto base_addr = peek_ab();
  return peek(base_addr) | (peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
}

auto Cpu::peek_indx() const -> uint16_t {
  const auto base_addr = peek_zpx();
  return (peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr);
}

auto Cpu::peek_indy() const -> uint16_t {
  const auto base_addr = peek_zp();
  return ((peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr)) + state.y;
}

//
// Snapshot
//

void Cpu::save(std::ofstream& out) const {
  dump_snapshot(out, ram);
  dump_snapshot(
    out,
    state.a,
    state.x,
    state.y,
    state.pc,
    state.sp,
    state.sr,
    state.ps,
    state.nmi_flag,
    state.irq_flag,
    state.cycle_count
  );
}

void Cpu::load(std::ifstream& in) {
  get_snapshot(in, ram);
  get_snapshot(
    in,
    state.a,
    state.x,
    state.y,
    state.pc,
    state.sp,
    state.sr,
    state.ps,
    state.nmi_flag,
    state.irq_flag,
    state.cycle_count
  );
}
}  // namespace nes
