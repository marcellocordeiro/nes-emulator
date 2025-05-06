#pragma once

#include "lib/common.hpp"

namespace nes::types::cpu {
enum class InterruptionType {
  Nmi,
  Rst,
  Irq,
  Brk,
};

enum class AddressingMode {
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
  IndirectY_Exception,
};

enum flags : u8 {
  Carry = 0x01,
  Zero = 0x02,
  Interrupt = 0x04,
  Decimal = 0x08,
  Break = 0x10,
  Reserved = 0x20,
  Overflow = 0x40,
  Negative = 0x80,
};

struct State {
  u8 a = 0;
  u8 x = 0;
  u8 y = 0;
  u16 pc = 0;
  u8 sp = 0;
  u8 sr = 0;

  u8 ps = 0;

  bool nmi_flag = false;
  bool irq_flag = false;

  i32 cycle_count = 0;

  [[nodiscard]] auto check_flags(u8) const -> bool;
  void set_flags(u8);
  void clear_flags(u8);
  void update_nz(u8);

  void set_a(u8);
  void set_x(u8);
  void set_y(u8);
  void set_pc(u16);
  void set_ps(u8);
};

namespace memory {
enum class Operation {
  None = -1,
  Read,
  Write,
};

enum class MemoryMap {
  Unknown = -1,
  CpuRam,
  PpuAccess,
  ApuAccess,
  OamDma,
  ControllerAccess,
  Controller1,
  Controller2,
  CartridgeAccess
};

template <auto Operation>
auto get_map(u16) -> MemoryMap;
} // namespace memory
} // namespace nes::types::cpu
