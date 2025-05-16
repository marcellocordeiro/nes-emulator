#include "mapper_7.hpp"

#include <stdexcept>

#include "../base_mapper.hpp"
#include "lib/common.hpp"

namespace nes {
void Mapper7::reset() {
  mode = 0;

  set_chr_map<8>(0, 0);
  apply();
}

void Mapper7::apply() {
  set_prg_map<32>(0, mode & 0x0F);

  set_mirroring((mode & 0x10) != 0 ? MirroringType::OneScreenHigh : MirroringType::OneScreenLow);
}

void Mapper7::write(const u16 addr, const u8 value) {
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 7 does not have PRG-RAM");
  }

  mode = value;
  apply();
}
} // namespace nes
