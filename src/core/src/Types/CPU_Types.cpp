#include "CPU_Types.h"

namespace nes::types::cpu {
auto state::check_flags(uint8_t flags) const -> bool { return (this->ps & flags) == flags; }

void state::set_flags(uint8_t flags) { this->ps |= flags; }

void state::clear_flags(uint8_t flags) { this->ps &= ~flags; }

void state::update_nz(uint8_t value)
{
  clear_flags(flags::Zero | flags::Negative);

  if (value == 0) {
    set_flags(flags::Zero);
  } else if (value & 0x80) {
    set_flags(flags::Negative);
  }
}

void state::set_a(uint8_t value)
{
  this->a = value;
  this->update_nz(this->a);
}

void state::set_x(uint8_t value)
{
  this->x = value;
  this->update_nz(this->x);
}

void state::set_y(uint8_t value)
{
  this->y = value;
  this->update_nz(this->y);
}

void state::set_pc(uint16_t addr) { this->pc = addr; }

void state::set_ps(uint8_t value) { this->ps = value & 0xCF; }

//
// CPU (memory)
//

namespace memory {
template <auto Operation> auto get_map(uint16_t addr) -> int
{
  auto in_range = [addr](const auto lower, const auto upper) { return (addr >= lower) && (addr <= upper); };

  if constexpr (Operation == Read) {
    if (addr <= 0x1FFF) {
      return CPU_RAM;
    }

    if (in_range(0x2000, 0x3FFF)) {
      return PPU_Access;
    }

    if (in_range(0x4000, 0x4013) || addr == 0x4015) {
      return APU_Access;
    }

    if (addr == 0x4016) {
      return Controller_1;
    }

    if (addr == 0x4017) {
      return Controller_2;
    }

    if (in_range(0x4018, 0x401F)) {
      return Unknown;
    }

    if (in_range(0x4020, 0x5FFF)) {
      return Unknown;
    }

    if (in_range(0x6000, 0xFFFF)) {
      return Cartridge_Access;
    }
  } else if constexpr (Operation == Write) {
    if (addr <= 0x1FFF) {
      return CPU_RAM;
    }

    if (in_range(0x2000, 0x3FFF)) {
      return PPU_Access;
    }

    if (in_range(0x4000, 0x4013) || addr == 0x4015) {
      return APU_Access;
    }

    if (addr == 0x4014) {
      return OAMDMA;
    }

    if (addr == 0x4016) {
      return Controller_Access;
    }

    if (addr == 0x4017) {
      return APU_Access;
    }

    if (in_range(0x4018, 0x401F)) {
      return Unknown;
    }

    if (in_range(0x4020, 0xFFFF)) {
      return Cartridge_Access;
    }
  }

  return Unknown;
}

template int get_map<Read>(uint16_t);
template int get_map<Write>(uint16_t);
}  // namespace memory
}  // namespace nes::types::cpu