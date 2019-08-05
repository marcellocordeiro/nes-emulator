#include "cpu.h"

#include <iostream>
#include <stdexcept>

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "emulator.h"
#include "log.h"
#include "ppu.h"

using namespace nes::types::cpu;

namespace nes {
cpu::cpu(nes::emulator& emulator_ref) : emulator(emulator_ref) {}

void cpu::power_on()
{
  state.cycle_count = 0;
  ram.fill(0);
  state.set_ps(0x34);
  INT_RST();
}

void cpu::reset()
{
  // todo
}

void cpu::set_nmi(bool value)
{
  state.nmi_flag = value;
}

void cpu::set_irq(bool value)
{
  state.irq_flag = value;
}

void cpu::dma_oam(uint8_t addr)
{
  for (uint16_t i = 0; i < 256; ++i) {
    // 0x2004 == OAMDATA
    memory_write(0x2004, memory_read((addr * 0x100) + i));
  }
}

int cpu::dmc_reader(uint16_t addr)
{
  return read(addr);
}

void cpu::run_frame()
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

  emulator.get_apu()->run_frame(state.cycle_count);
}

void cpu::tick()
{
  emulator.get_ppu()->step();
  emulator.get_ppu()->step();
  emulator.get_ppu()->step();
  ++state.cycle_count;
}

uint8_t cpu::read(uint16_t addr) const
{
  using namespace nes::types::cpu::memory;

  switch (get_map<Read>(addr)) {
    case CPU_RAM: return ram[addr % 0x800];
    case PPU_Access: return emulator.get_ppu()->read(addr);
    case APU_Access: return emulator.get_apu()->read(state.cycle_count);
    case Controller_1: return emulator.get_controller()->read(0);
    case Controller_2: return emulator.get_controller()->read(1);
    case Cartridge: return emulator.get_cartridge()->prg_read(addr);
    case Unknown:
    default:
      LOG(lib::log::Error) << "Invalid read address:" << std::showbase
                           << std::hex << addr;
      return 0;
  }
}

void cpu::write(uint16_t addr, uint8_t value)
{
  using namespace nes::types::cpu::memory;

  switch (get_map<Write>(addr)) {
    case CPU_RAM: ram[addr % 0x800] = value; break;
    case PPU_Access: emulator.get_ppu()->write(addr, value); break;
    case APU_Access:
      emulator.get_apu()->write(state.cycle_count, addr, value);
      break;
    case OAMDMA: dma_oam(value); break;
    case Controller: emulator.get_controller()->write(value & 1); break;
    case Cartridge: emulator.get_cartridge()->prg_write(addr, value); break;
    default: throw std::runtime_error("Invalid write address");
  }
}

uint8_t cpu::memory_read(uint16_t addr)
{
  tick();
  return read(addr);
}

void cpu::memory_write(uint16_t addr, uint8_t value)
{
  tick();
  write(addr, value);
}

uint8_t cpu::peek(uint16_t addr) const
{
  return read(addr);
}

uint16_t cpu::peek_imm() const
{
  return state.pc + 1;
}

uint16_t cpu::peek_rel() const
{
  return state.pc + 1;
}

uint16_t cpu::peek_zp() const
{
  return peek(peek_imm());
}

uint16_t cpu::peek_zpx() const
{
  return (peek_zp() + state.x) & 0xFF;
}

uint16_t cpu::peek_zpy() const
{
  return (peek_zp() + state.y) & 0xFF;
}

uint16_t cpu::peek_ab() const
{
  const auto base_addr = peek_imm();
  return (peek(base_addr + 1) << 8) | peek(base_addr);
}

uint16_t cpu::peek_abx() const
{
  const auto base_addr = peek_ab();
  return base_addr + state.x;
}

uint16_t cpu::peek_aby() const
{
  const auto base_addr = peek_ab();
  return base_addr + state.y;
}

uint16_t cpu::peek_ind() const
{
  const auto base_addr = peek_ab();
  return peek(base_addr) |
         (peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
}

uint16_t cpu::peek_indx() const
{
  const auto base_addr = peek_zpx();
  return (peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr);
}

uint16_t cpu::peek_indy() const
{
  const auto base_addr = peek_zp();
  return ((peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr)) + state.y;
}

//
// Snapshot
//

void cpu::save(std::ofstream& out)
{
  for (const auto& value : ram) dump_snapshot(out, value);
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
      state.cycle_count);
}

void cpu::load(std::ifstream& in)
{
  for (auto& value : ram) get_snapshot(in, value);
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
      state.cycle_count);
}
}  // namespace nes
