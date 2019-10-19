#include "Debugger.h"

#include <array>
#include <sstream>
#include <string_view>

#include <fmt/format.h>

#include <common.h>
#include "CPU.h"
#include "PPU.h"
#include "Types/CPU_Types.h"

#include <iomanip> // TODO: remove this

namespace nes {
using namespace types::cpu::addressing_mode;

// debugger::debugger(emulator& emu_ref) : emu(emu_ref) {}

void debugger::cpu_log()
{
  // clang-format off
  constexpr std::array<std::string_view, 0x100> instruction = {
      // 0     1      2      3       4       5      6      7       8      9      A       B       C       D      E      F
      "BRK",  "ORA", "inv", "*SLO", "*NOP", "ORA", "ASL", "*SLO", "PHP", "ORA", "ASL",  "inv",  "*NOP", "ORA", "ASL", "*SLO",  // 0
      "BPL",  "ORA", "inv", "*SLO", "*NOP", "ORA", "ASL", "*SLO", "CLC", "ORA", "*NOP", "*SLO", "*NOP", "ORA", "ASL", "*SLO",  // 1
      "JSR",  "AND", "inv", "*RLA", "BIT",  "AND", "ROL", "*RLA", "PLP", "AND", "ROL",  "inv",  "BIT",  "AND", "ROL", "*RLA",  // 2
      "BMI",  "AND", "inv", "*RLA", "*NOP", "AND", "ROL", "*RLA", "SEC", "AND", "*NOP", "*RLA", "*NOP", "AND", "ROL", "*RLA",  // 3
      "RTI",  "EOR", "inv", "*SRE", "*NOP", "EOR", "LSR", "*SRE", "PHA", "EOR", "LSR",  "inv",  "JMP",  "EOR", "LSR", "*SRE",  // 4
      "BVC",  "EOR", "inv", "*SRE", "*NOP", "EOR", "LSR", "*SRE", "CLI", "EOR", "*NOP", "*SRE", "*NOP", "EOR", "LSR", "*SRE",  // 5
      "RTS",  "ADC", "inv", "*RRA", "*NOP", "ADC", "ROR", "*RRA", "PLA", "ADC", "ROR",  "inv",  "JMP",  "ADC", "ROR", "*RRA",  // 6
      "BVS",  "ADC", "inv", "*RRA", "*NOP", "ADC", "ROR", "*RRA", "SEI", "ADC", "*NOP", "*RRA", "*NOP", "ADC", "ROR", "*RRA",  // 7
      "*NOP", "STA", "inv", "*SAX", "STY",  "STA", "STX", "*SAX", "DEY", "inv", "TXA",  "inv",  "STY",  "STA", "STX", "*SAX",  // 8
      "BCC",  "STA", "inv", "inv",  "STY",  "STA", "STX", "*SAX", "TYA", "STA", "TXS",  "inv",  "inv",  "STA", "inv", "inv",   // 9
      "LDY",  "LDA", "LDX", "*LAX", "LDY",  "LDA", "LDX", "*LAX", "TAY", "LDA", "TAX",  "inv",  "LDY",  "LDA", "LDX", "*LAX",  // A
      "BCS",  "LDA", "inv", "*LAX", "LDY",  "LDA", "LDX", "*LAX", "CLV", "LDA", "TSX",  "inv",  "LDY",  "LDA", "LDX", "*LAX",  // B
      "CPY",  "CMP", "inv", "*DCP", "CPY",  "CMP", "DEC", "*DCP", "INY", "CMP", "DEX",  "inv",  "CPY",  "CMP", "DEC", "*DCP",  // C
      "BNE",  "CMP", "inv", "*DCP", "*NOP", "CMP", "DEC", "*DCP", "CLD", "CMP", "*NOP", "*DCP", "*NOP", "CMP", "DEC", "*DCP",  // D
      "CPX",  "SBC", "inv", "*ISB", "CPX",  "SBC", "INC", "*ISB", "INX", "SBC", "NOP",  "*SBC", "CPX",  "SBC", "INC", "*ISB",  // E
      "BEQ",  "SBC", "inv", "*ISB", "*NOP", "SBC", "INC", "*ISB", "SED", "SBC", "*NOP", "*ISB", "*NOP", "SBC", "INC", "*ISB",  // F
  };

  enum addr_mode2 { impl, acc, imm, zp, zpx, zpy, rel, ab, abx, abx_, aby, aby_, ind, indx, indy, indy_, inv };

  constexpr std::array<int, 0x100> addr_mode = {
      // 0  1     2    3     4    5    6    7    8     9    A     B    C    D    E    F
      impl, indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, acc,  inv, ab,  ab,  ab,  ab,   // 0
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // 1
      ab,   indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, acc,  inv, ab,  ab,  ab,  ab,   // 2
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // 3
      impl, indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, acc,  inv, ab,  ab,  ab,  ab,   // 4
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // 5
      impl, indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, acc,  inv, ind, ab,  ab,  ab,   // 6
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // 7
      imm,  indx, inv, indx, zp,  zp,  zp,  zp,  impl, inv, impl, inv, ab,  ab,  ab,  ab,   // 8
      rel,  indy, inv, inv,  zpx, zpx, zpy, zpy, impl, aby, impl, inv, inv, abx, inv, inv,  // 9
      imm,  indx, imm, indx, zp,  zp,  zp,  zp,  impl, imm, impl, inv, ab,  ab,  ab,  ab,   // A
      rel,  indy, inv, indy, zpx, zpx, zpy, zpy, impl, aby, impl, inv, abx, abx, aby, aby,  // B
      imm,  indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, impl, inv, ab,  ab,  ab,  ab,   // C
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // D
      imm,  indx, inv, indx, zp,  zp,  zp,  zp,  impl, imm, impl, imm, ab,  ab,  ab,  ab,   // E
      rel,  indy, inv, indy, zpx, zpx, zpx, zpx, impl, aby, impl, aby, abx, abx, abx, abx,  // F
  };
  // clang-format on

  auto* cpu_ptr = &cpu::get();

  auto peek      = [&](uint16_t addr) { return cpu_ptr->peek(addr); };
  auto peek_imm  = [&]() { return cpu_ptr->peek_imm(); };
  auto peek_rel  = [&]() { return cpu_ptr->peek_rel(); };
  auto peek_zp   = [&]() { return cpu_ptr->peek_zp(); };
  auto peek_zpx  = [&]() { return cpu_ptr->peek_zpx(); };
  auto peek_zpy  = [&]() { return cpu_ptr->peek_zpy(); };
  auto peek_ab   = [&]() { return cpu_ptr->peek_ab(); };
  auto peek_abx  = [&]() { return cpu_ptr->peek_abx(); };
  auto peek_aby  = [&]() { return cpu_ptr->peek_aby(); };
  auto peek_ind  = [&]() { return cpu_ptr->peek_ind(); };
  auto peek_indx = [&]() { return cpu_ptr->peek_indx(); };
  auto peek_indy = [&]() { return cpu_ptr->peek_indy(); };

  auto read_word_zp = [&](uint16_t addr) -> uint16_t {
    return peek((addr + 1) & 0xFF) << 8 | peek(addr);
  };

  auto state = cpu_ptr->get_state();

  std::stringstream ss;

  const auto inst   = instruction[peek(state.pc)];
  const auto addr_m = addr_mode[peek(state.pc)];

  ss << fmt::format("{:04X}  {:02X} ", state.pc, peek(state.pc));

  uint8_t  arg8   = peek(state.pc + 1);
  uint8_t  arg8_2 = peek(state.pc + 2);
  uint16_t arg16  = (arg8_2 << 8) | arg8;

  switch (addr_m) {
    case Absolute:
    case AbsoluteX:
    case AbsoluteY:
    case Indirect: ss << fmt::format("{:02X} {:02X} ", arg8, arg8_2); break;
    case IndirectY:
    case IndirectX:
    case ZeroPage:
    case ZeroPageX:
    case ZeroPageY:
    case Relative:
    case Immediate: ss << fmt::format("{:02X}    ", arg8); break;
    default: ss << "      "; break;
  }

  ss << fmt::format("{:>4s} ", inst);

  ss << std::left << std::setw(28) << std::setfill(' ');

  switch (addr_m) {
    case Implicit: {
      ss << " ";
      break;
    }
    case Accumulator:
      if (inst == "LSR" || inst == "ASL" || inst == "ROR" || inst == "ROL") {
        ss << "A";
      } else {
        ss << " ";
      }
      break;
    case Immediate: {
      ss << fmt::format("#${:02X}", peek(peek_imm()));
      break;
    }
    case Relative: {
      ss << fmt::format("${:04X}", peek_rel());
      break;
    }
    case ZeroPage: {
      ss << fmt::format("${:02X} = {:02X}", peek_zp(), peek(peek_zp()));
      break;
    }
    case ZeroPageX: {
      ss << fmt::format("${:02X},X @ {:02X} = {:02X}", peek_zp(), peek_zpx(),
                        peek(peek_zpx()));
      break;
    }
    case ZeroPageY: {
      ss << fmt::format("${:02X},Y @ {:02X} = {:02X}", peek_zp(), peek_zpy(),
                        peek(peek_zpy()));
      break;
    }
    case Absolute: {
      if (inst == "JMP" || inst == "JSR") {
        ss << fmt::format("${:04X}", peek_ab());
      } else {
        ss << fmt::format("${:04X} = {:02X}", peek_ab(), peek(peek_ab()));
      }
      break;
    }
    case AbsoluteX: {
      ss << fmt::format("${:04X},X @ {:04X} = {:02X}", arg16, peek_abx(),
                        peek(peek_abx()));
      break;
    }
    case AbsoluteY: {
      ss << fmt::format("${:04X},Y @ {:04X} = {:02X}", arg16, peek_aby(),
                        peek(peek_aby()));
      break;
    }
    case Indirect: {
      ss << fmt::format("(${:04X}) = {:04X}", peek_ab(), peek_ind());
      break;
    }
    case IndirectX: {
      ss << fmt::format("(${:02X},X) @ {:02X} = {:04X} = {:02X}", arg8,
                        peek_zpx(), peek_indx(), peek(peek_indx()));
      break;
    }
    case IndirectY: {
      ss << fmt::format("(${:02X}),Y = {:04X} @ {:04X} = {:02X}", arg8,
                        read_word_zp(arg8), peek_indy(), peek(peek_indy()));
      break;
    }
    default: ss << " "; break;
  }

  auto ppu_cycle    = ppu::get().cycle_count();
  auto ppu_scanline = ppu::get().scanline_count();

  ss << fmt::format(
      "A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X} PPU:{:3d},{:3d} "
      "CYC:{:d}\n",
      state.a, state.x, state.y, (state.ps | 0x20), state.sp, ppu_cycle,
      ppu_scanline, state.cycle_count);

  nestest_log << ss.str();

  if (state.pc == 0xC66E) {
    nestest_log.close();
    exit(0);
  }
}
}  // namespace nes
