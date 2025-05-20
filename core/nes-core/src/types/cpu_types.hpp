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

enum Flags {
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

  [[nodiscard]] auto check_flags(u8 flags) const -> bool;
  void set_flags(u8 flags);
  void clear_flags(u8 flags);
  void update_nz(u8 value);

  void set_a(u8 value);
  void set_x(u8 value);
  void set_y(u8 value);
  void set_pc(u16 addr);
  void set_ps(u8 value);
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
auto get_map(u16 addr) -> MemoryMap;
} // namespace memory
} // namespace nes::types::cpu
