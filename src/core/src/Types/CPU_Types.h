#pragma once

#include <common.h>

namespace nes::types::cpu {
namespace interruption_type {
enum interruption_type { NMI, RST, IRQ, BRK };
}

namespace addressing_mode {
enum addressing_mode {
  Invalid = -1,
  Implicit,
  Accumulator,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Relative,
  Absolute,
  AbsoluteX,
  AbsoluteX_Exception,
  AbsoluteY,
  AbsoluteY_Exception,
  Indirect,
  IndirectX,
  IndirectY,
  IndirectY_Exception
};
}

namespace flags {
enum flags : uint8_t {
  Carry     = 0x01,
  Zero      = 0x02,
  Interrupt = 0x04,
  Decimal   = 0x08,
  Break     = 0x10,
  Reserved  = 0x20,
  Overflow  = 0x40,
  Negative  = 0x80
};
}

struct state {
  uint8_t  a  = 0;
  uint8_t  x  = 0;
  uint8_t  y  = 0;
  uint16_t pc = 0;
  uint8_t  sp = 0;
  uint8_t  sr = 0;

  uint8_t ps = 0;

  bool nmi_flag = false;
  bool irq_flag = false;

  int cycle_count = 0;

  auto check_flags(uint8_t) const -> bool;
  void set_flags(uint8_t);
  void clear_flags(uint8_t);
  void update_nz(uint8_t);

  void set_a(uint8_t);
  void set_x(uint8_t);
  void set_y(uint8_t);
  void set_pc(uint16_t);
  void set_ps(uint8_t);
};

namespace memory {
enum operation { None = -1, Read, Write };

enum map {
  Unknown = -1,
  CPU_RAM,
  PPU_Access,
  APU_Access,
  OAMDMA,
  Controller_Access,
  Controller_1,
  Controller_2,
  Cartridge_Access
};

template <auto Operation> auto get_map(uint16_t) -> int;
}  // namespace memory
}  // namespace nes::types::cpu
