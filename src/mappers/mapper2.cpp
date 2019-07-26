#include "mappers/mapper2.h"

namespace nes {
mapper2::mapper2(nes::cartridge& cartridge_ref) : mapper{cartridge_ref} {}

void mapper2::reset()
{
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, -1);
  set_chr_map<8>(0, 0);
}

void mapper2::prg_write(const uint16_t addr, const uint8_t value)
{
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 2 does not have PRG-RAM");
  }
  
  set_prg_map<16>(0, value);
}
}  // namespace nes
