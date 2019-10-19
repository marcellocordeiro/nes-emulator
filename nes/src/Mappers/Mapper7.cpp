#include "Mapper7.h"

#include "../PPU.h"
#include "../Types/Cartridge_Types.h"

namespace nes {
void mapper7::reset()
{
  mode = 0;

  set_chr_map<8>(0, 0);
  apply();
}

void mapper7::apply()
{
  set_prg_map<32>(0, mode & 0x0F);

  using namespace mirroring;
  ppu::get().set_mirroring((mode & 0x10) ? One_Screen_High : One_Screen_Low);
}

void mapper7::prg_write(uint16_t addr, uint8_t value)
{
  if (addr < 0x8000) {
    throw std::runtime_error("Mapper 7 does not have PRG-RAM");
  }

  mode = value;
  apply();
}

void mapper7::save(std::ofstream& out)
{
  base_mapper::save(out);

  dump_snapshot(out, mode);
}

void mapper7::load(std::ifstream& in)
{
  base_mapper::load(in);

  get_snapshot(in, mode);

  apply();
}
}  // namespace nes
