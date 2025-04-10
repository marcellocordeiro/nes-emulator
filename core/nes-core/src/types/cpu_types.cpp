#include "cpu_types.h"

namespace nes::types::cpu {
auto State::check_flags(uint8_t flags) const -> bool {
  return (this->ps & flags) == flags;
}

void State::set_flags(uint8_t flags) {
  this->ps |= flags;
}

void State::clear_flags(uint8_t flags) {
  this->ps &= ~flags;
}

void State::update_nz(uint8_t value) {
  clear_flags(flags::Zero | flags::Negative);

  if (value == 0) {
    set_flags(flags::Zero);
  } else if (value & 0x80) {
    set_flags(flags::Negative);
  }
}

void State::set_a(uint8_t value) {
  this->a = value;
  this->update_nz(this->a);
}

void State::set_x(uint8_t value) {
  this->x = value;
  this->update_nz(this->x);
}

void State::set_y(uint8_t value) {
  this->y = value;
  this->update_nz(this->y);
}

void State::set_pc(uint16_t addr) {
  this->pc = addr;
}

void State::set_ps(uint8_t value) {
  this->ps = value & 0xCF;
}

//
// CPU (memory)
//

namespace memory {
template <auto Operation>
auto get_map(uint16_t addr) -> MemoryMap {
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

template MemoryMap get_map<Operation::Read>(uint16_t);
template MemoryMap get_map<Operation::Write>(uint16_t);
} // namespace memory
} // namespace nes::types::cpu
