#include "CPU.h"

#include <spdlog/spdlog.h>

#include "Cartridge.h"
#include "Controller.h"
#include "PPU.h"

using namespace nes::types::cpu;

namespace nes {
CPU& CPU::get()
{
  static CPU instance;
  return instance;
}

void CPU::power_on()
{
  state.a  = 0;
  state.x  = 0;
  state.y  = 0;
  state.pc = (peek(0xFFFC + 1) << 8) | peek(0xFFFC);
  state.sp = 0xFD;

  state.cycle_count = 0;
  state.set_ps(0x34);
  ram.fill(0);

  // nestest
  // state.pc          = 0xC000;
  // state.cycle_count = 7;
}

void CPU::reset() { INT_RST(); }

void CPU::set_nmi(bool value) { state.nmi_flag = value; }

void CPU::set_irq(bool value) { state.irq_flag = value; }

void CPU::dma_oam(uint8_t addr)
{
  for (uint16_t i = 0; i < 256; ++i) {
    // 0x2004 == OAMDATA
    memory_write(0x2004, memory_read((addr * 0x100) + i));
  }
}

void CPU::run_frame()
{
  constexpr auto cycles_per_frame = 29781;

  state.cycle_count %= cycles_per_frame;

  while (state.cycle_count < cycles_per_frame) {
    if (state.nmi_flag) {
      INT_NMI();
    } else if (state.irq_flag && !state.check_flags(flags::Interrupt)) {
      INT_IRQ();
    }

    execute();
  }
}

void CPU::tick()
{
  PPU::get().step();
  PPU::get().step();
  PPU::get().step();
  ++state.cycle_count;
}

uint8_t CPU::peek(uint16_t addr) const
{
  using namespace nes::types::cpu::memory;

  switch (get_map<Read>(addr)) {
    case CPU_RAM: return ram[addr & 0x07FF];
    case PPU_Access: return PPU::get().peek_reg(addr);
    case APU_Access:
      return 0xFF;  // Avoids APU side effects and satisfies nestest
    case Controller_1: return Controller::get().peek(0);
    case Controller_2: return Controller::get().peek(1);
    case Cartridge_Access: return Cartridge::get().prg_read(addr);
    case Unknown:
    default: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;
  }
}

uint8_t CPU::read(uint16_t addr) const
{
  using namespace nes::types::cpu::memory;

  switch (get_map<Read>(addr)) {
    case CPU_RAM: return ram[addr & 0x07FF];
    case PPU_Access: return PPU::get().read(addr);
    case APU_Access: return 0; // APU::get().read(...);
    case Controller_1: return Controller::get().read(0);
    case Controller_2: return Controller::get().read(1);
    case Cartridge_Access: return Cartridge::get().prg_read(addr);
    case Unknown:
    default: SPDLOG_ERROR("Invalid read address: 0x{:04X}", addr); return 0;
  }
}

void CPU::write(uint16_t addr, uint8_t value)
{
  using namespace nes::types::cpu::memory;

  switch (get_map<Write>(addr)) {
    case CPU_RAM: ram[addr & 0x07FF] = value; break;
    case PPU_Access: PPU::get().write(addr, value); break;
    case APU_Access: break; // APU::get().write(...); break;
    case OAMDMA: dma_oam(value); break;
    case Controller_Access: Controller::get().write(value & 1); break;
    case Cartridge_Access: Cartridge::get().prg_write(addr, value); break;
    default: throw std::runtime_error("Invalid write address");
  }
}

uint8_t CPU::memory_read(uint16_t addr)
{
  tick();
  return read(addr);
}

void CPU::memory_write(uint16_t addr, uint8_t value)
{
  tick();
  write(addr, value);
}

types::cpu::state CPU::get_state() const { return state; }

uint16_t CPU::peek_imm() const { return state.pc + 1; }

uint16_t CPU::peek_rel() const
{
  auto addr   = peek_imm();
  auto offset = static_cast<int8_t>(peek(addr));

  return (state.pc + 2) + offset;
}

uint16_t CPU::peek_zp() const { return peek(peek_imm()); }

uint16_t CPU::peek_zpx() const { return (peek_zp() + state.x) & 0xFF; }

uint16_t CPU::peek_zpy() const { return (peek_zp() + state.y) & 0xFF; }

uint16_t CPU::peek_ab() const
{
  const auto base_addr = peek_imm();
  return (peek(base_addr + 1) << 8) | peek(base_addr);
}

uint16_t CPU::peek_abx() const
{
  const auto base_addr = peek_ab();
  return base_addr + state.x;
}

uint16_t CPU::peek_aby() const
{
  const auto base_addr = peek_ab();
  return base_addr + state.y;
}

uint16_t CPU::peek_ind() const
{
  const auto base_addr = peek_ab();
  return peek(base_addr) |
         (peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
}

uint16_t CPU::peek_indx() const
{
  const auto base_addr = peek_zpx();
  return (peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr);
}

uint16_t CPU::peek_indy() const
{
  const auto base_addr = peek_zp();
  return ((peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr)) + state.y;
}

//
// Snapshot
//

void CPU::save(std::ofstream& out)
{
  dump_snapshot(out, ram);
  dump_snapshot(out, state.a, state.x, state.y, state.pc, state.sp, state.sr,
                state.ps, state.nmi_flag, state.irq_flag, state.cycle_count);
}

void CPU::load(std::ifstream& in)
{
  get_snapshot(in, ram);
  get_snapshot(in, state.a, state.x, state.y, state.pc, state.sp, state.sr,
               state.ps, state.nmi_flag, state.irq_flag, state.cycle_count);
}
}  // namespace nes
