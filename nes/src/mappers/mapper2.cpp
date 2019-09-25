#include "nes/mappers/mapper2.h"

namespace nes {
mapper2::mapper2(cartridge& cart_ref) : base_mapper(cart_ref) {}

void mapper2::reset()
{
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, -1);
  set_chr_map<8>(0, 0);
}

void mapper2::apply()
{
  set_prg_map<16>(0, mode);
}

void mapper2::prg_write(uint16_t addr, uint8_t value)
{
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 2 does not have PRG-RAM");
  }

  mode = value;
  apply();
}

void mapper2::save(std::ofstream& out)
{
  base_mapper::save(out);

  dump_snapshot(out, mode);
}

void mapper2::load(std::ifstream& in)
{
  base_mapper::load(in);

  get_snapshot(in, mode);

  apply();
}
}  // namespace nes
