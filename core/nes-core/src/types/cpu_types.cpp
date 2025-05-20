#include "cpu_types.hpp"

#include "lib/common.hpp"

namespace nes::types::cpu {
auto State::check_flags(const u8 flags) const -> bool {
  return (this->ps & flags) == flags;
}

void State::set_flags(const u8 flags) {
  this->ps |= flags;
}

void State::clear_flags(const u8 flags) {
  this->ps &= ~flags;
}

void State::update_nz(const u8 value) {
  clear_flags(Flags::Zero | Flags::Negative);

  if (value == 0) {
    set_flags(Flags::Zero);
  } else if ((value & 0x80) != 0) {
    set_flags(Flags::Negative);
  }
}

void State::set_a(const u8 value) {
  this->a = value;
  this->update_nz(this->a);
}

void State::set_x(const u8 value) {
  this->x = value;
  this->update_nz(this->x);
}

void State::set_y(const u8 value) {
  this->y = value;
  this->update_nz(this->y);
}

void State::set_pc(const u16 addr) {
  this->pc = addr;
}

void State::set_ps(const u8 value) {
  this->ps = value & 0xCF;
}

//
// CPU (memory)
//

namespace memory {
template <auto Operation>
auto get_map(u16 addr) -> MemoryMap { // NOLINT
  auto in_range = [addr](const auto lower, const auto upper) {
    return (addr >= lower) && (addr <= upper);
  };

  if constexpr (Operation == Operation::Read) {
    if (addr <= 0x1FFF) {
      return MemoryMap::CpuRam;
    }

    if (in_range(0x2000, 0x3FFF)) {
      return MemoryMap::PpuAccess;
    }

    if (in_range(0x4000, 0x4013) || addr == 0x4015) {
      return MemoryMap::ApuAccess;
    }

    if (addr == 0x4016) {
      return MemoryMap::Controller1;
    }

    if (addr == 0x4017) {
      return MemoryMap::Controller2;
    }

    if (in_range(0x4018, 0x401F)) {
      return MemoryMap::Unknown;
    }

    if (in_range(0x4020, 0x5FFF)) {
      return MemoryMap::Unknown;
    }

    if (in_range(0x6000, 0xFFFF)) {
      return MemoryMap::CartridgeAccess;
    }
  } else if constexpr (Operation == Operation::Write) {
    if (addr <= 0x1FFF) {
      return MemoryMap::CpuRam;
    }

    if (in_range(0x2000, 0x3FFF)) {
      return MemoryMap::PpuAccess;
    }

    if (in_range(0x4000, 0x4013) || addr == 0x4015) {
      return MemoryMap::ApuAccess;
    }

    if (addr == 0x4014) {
      return MemoryMap::OamDma;
    }

    if (addr == 0x4016) {
      return MemoryMap::ControllerAccess;
    }

    if (addr == 0x4017) {
      return MemoryMap::ApuAccess;
    }

    if (in_range(0x4018, 0x401F)) {
      return MemoryMap::Unknown;
    }

    if (in_range(0x4020, 0xFFFF)) {
      return MemoryMap::CartridgeAccess;
    }
  }

  return MemoryMap::Unknown;
}

template auto get_map<Operation::Read>(u16) -> MemoryMap;
template auto get_map<Operation::Write>(u16) -> MemoryMap;
} // namespace memory
} // namespace nes::types::cpu
