#include "debugger.h"

#if 0
#include <array>
#include <iomanip>
#include <sstream>
#include <string_view>

#include <fmt/format.h>

namespace nes {
void debugger::nestest() {
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

  constexpr std::array<addr_mode2, 0x100> addressing_mode = {
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

  auto read_word    = [this](const uint16_t addr) -> uint16_t { return cpu.read(addr + 1) << 8 | cpu.read(addr); };
  auto read_word_zp = [this](const uint16_t addr) -> uint16_t { return cpu.read((addr + 1) & 0xFF) << 8 | cpu.read(addr); };

  std::stringstream ss;

  const auto inst   = instruction[cpu.read(cpu.state.pc)];
  const auto addr_m = addressing_mode[cpu.read(cpu.state.pc)];

  ss << fmt::format("{:04X}  {:02X} ", cpu.state.pc, cpu.read(cpu.state.pc));

  uint8_t  arg8   = cpu.read(cpu.state.pc + 1);
  uint8_t  arg8_2 = cpu.read(cpu.state.pc + 2);
  uint16_t arg16  = arg8 | (arg8_2 << 8);

  switch (addr_m) {
    case addr_mode2::ab:
    case addr_mode2::abx:
    case addr_mode2::aby:
    case addr_mode2::ind: ss << fmt::format("{:02X} {:02X} ", arg8, arg8_2); break;
    case addr_mode2::indy:
    case addr_mode2::indx:
    case addr_mode2::zp:
    case addr_mode2::zpx:
    case addr_mode2::zpy:
    case addr_mode2::rel:
    case addr_mode2::imm: ss << fmt::format("{:02X}    ", arg8); break;
    default: ss << "      "; break;
  }

  ss << fmt::format("{:>4s} ", inst);

  ss << std::left << std::setw(28) << std::setfill(' ');

  switch (addr_m) {
    case addr_mode2::impl: {
      ss << " ";
      break;
    }
    case addr_mode2::acc:
      if (inst == "LSR" || inst == "ASL" || inst == "ROR" || inst == "ROL") {
        ss << "A";
      } else {
        ss << " ";
      }
      break;
    case addr_mode2::imm: {
      const auto addr = cpu.peek_imm();
      ss << fmt::format("#${:02X}", cpu.peek(addr));
      break;
    }
    case addr_mode2::zp: {
      const auto addr = cpu.peek(cpu.peek_imm());
      ss << fmt::format("${:02X} = {:02X}", addr, cpu.peek(addr));
      break;
    }
    case addr_mode2::zpx: {
      const auto zp_addr = cpu.peek_zp();
      const auto addr    = cpu.peek_zpx();  // (zp_addr + state.x) & 0xFF
      ss << fmt::format("${:02X},X @ {:02X} = {:02X}", zp_addr, addr, cpu.peek(addr));
      break;
    }
    case addr_mode2::zpy: {
      const auto zp_addr = cpu.peek_zp();
      const auto addr    = cpu.peek_zpy();
      ss << fmt::format("${:02X},Y @ {:02X} = {:02X}", zp_addr, addr, cpu.peek(addr));
      break;
    }
    case addr_mode2::rel: {
      const uint16_t addr = cpu.state.pc + 2 + static_cast<int8_t>(cpu.peek(cpu.peek_imm()));
      ss << fmt::format("${:04X}", addr);
      break;
    }
    case addr_mode2::ab: {
      const auto addr = read_word(cpu.peek_imm());
      
      if (inst == "JMP" || inst == "JSR") {
        ss << fmt::format("${:04X}", addr);
      } else {
        // read about this
        ss << fmt::format("${:04X} = {:02X}", addr, cpu.peek(addr));
      }
      break;
    }
    case addr_mode2::abx: {
      ss << fmt::format("${:04X},X @ {:04X} = {:02X}", arg16, uint16_t(arg16 + cpu.state.x), cpu.read(arg16 + cpu.state.x));
      break;
    }
    case addr_mode2::aby: {
      ss << fmt::format("${:04X},Y @ {:04X} = {:02X}", arg16, uint16_t(arg16 + cpu.state.y), cpu.read(arg16 + cpu.state.y));
      break;
    }
    case addr_mode2::ind: {
      // read about this
      const uint16_t base_addr = cpu.peek_ab();
      const uint16_t addr      = cpu.peek_ind();
      ss << fmt::format("(${:04X}) = {:04X}", base_addr, addr);
      break;
    }
    case addr_mode2::indx: {
      ss << fmt::format("(${:02X},X) @ {:02X} = {:04X} = {:02X}", arg8, uint8_t(cpu.state.x + arg8), read_word_zp((cpu.state.x + arg8) % 0x100), cpu.read(read_word_zp((cpu.state.x + arg8) % 0x100)));
      break;
    }
    case addr_mode2::indy: {
      ss << fmt::format("(${:02X}),Y = {:04X} @ {:04X} = {:02X}", arg8, read_word_zp(arg8), uint16_t(read_word_zp(arg8) + cpu.state.y), cpu.read(read_word_zp(arg8) + cpu.state.y));
      break;
    }
    default: ss << " "; break;
  }

  ss << fmt::format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X} CYC:{:3d}\n", cpu.state.a, cpu.state.x, cpu.state.y, (cpu.state.ps | 0x20), cpu.state.sp, (cpu.state.cycle_count - 7) * 3 % 341);

  nestest_log << ss.str();

  if (cpu.state.pc == 0xC66E) {
    nestest_log.close();
    exit(0);
  }
}
}  // namespace nes
#endif
