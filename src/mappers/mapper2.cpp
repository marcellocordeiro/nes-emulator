#include "mappers/mapper2.h"

namespace nes {
mapper2::mapper2(nes::cartridge& cartridge_ref) : mapper(cartridge_ref) {}

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

void mapper2::prg_write(const uint16_t addr, const uint8_t value)
{
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 2 does not have PRG-RAM");
  }

  mode = value;
  apply();
}

void mapper2::save(std::ofstream& out)
{
  for (const auto& value : prg_ram) dump_snapshot(out, value);
  for (const auto& value : chr) dump_snapshot(out, value);
  dump_snapshot(out, mode);
}

void mapper2::load(std::ifstream& in)
{
  for (auto& value : prg_ram) get_snapshot(in, value);
  for (auto& value : chr) get_snapshot(in, value);
  get_snapshot(in, mode);

  apply();
}
}  // namespace nes
