#include "mapper_2.hpp"

#include <stdexcept>

#include "lib/common.hpp"

namespace nes {
void Mapper2::reset() {
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, -1);
  set_chr_map<8>(0, 0);
}

void Mapper2::apply() {
  set_prg_map<16>(0, mode);
}

void Mapper2::write(const u16 addr, const u8 value) {
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 2 does not have PRG-RAM");
  }

  mode = value;
  apply();
}
} // namespace nes
