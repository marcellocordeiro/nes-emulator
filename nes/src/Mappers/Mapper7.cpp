#include "Mapper7.h"

#include "../PPU.h"
#include "../Types/Cartridge_Types.h"

namespace nes {
void Mapper7::reset()
{
  mode = 0;

  set_chr_map<8>(0, 0);
  apply();
}

void Mapper7::apply()
{
  set_prg_map<32>(0, mode & 0x0F);

  using namespace mirroring;
  PPU::get().set_mirroring((mode & 0x10) ? One_Screen_High : One_Screen_Low);
}

void Mapper7::prg_write(uint16_t addr, uint8_t value)
{
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 7 does not have PRG-RAM");
  }

  mode = value;
  apply();
}

void Mapper7::save(std::ofstream& out)
{
  BaseMapper::save(out);

  dump_snapshot(out, mode);
}

void Mapper7::load(std::ifstream& in)
{
  BaseMapper::load(in);

  get_snapshot(in, mode);

  apply();
}
}  // namespace nes
