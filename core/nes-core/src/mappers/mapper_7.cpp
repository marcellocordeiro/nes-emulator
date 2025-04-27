#include "mapper_7.h"

namespace nes {
void Mapper7::reset() {
  mode = 0;

  set_chr_map<8>(0, 0);
  apply();
}

void Mapper7::apply() {
  set_prg_map<32>(0, mode & 0x0F);

  set_mirroring((mode & 0x10) ? MirroringType::OneScreenHigh : MirroringType::OneScreenLow);
}

void Mapper7::write(u16 addr, u8 value) {
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 7 does not have PRG-RAM");
  }

  mode = value;
  apply();
}

void Mapper7::save(std::ofstream& out) const {
  BaseMapper::save(out);

  dump_snapshot(out, mode);
}

void Mapper7::load(std::ifstream& in) {
  BaseMapper::load(in);

  get_snapshot(in, mode);

  apply();
}
} // namespace nes
