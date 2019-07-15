#include "mappers/mapper0.h"

namespace nes {
mapper0::mapper0(
    const nes::cartridge_info& info_in,
    std::vector<uint8_t>&&     prg_in,
    std::vector<uint8_t>&&     chr_in)
  : mapper(info_in, std::move(prg_in), std::move(chr_in))
{}

void mapper0::reset()
{
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, 1);
  set_chr_map<8>(0, 0);

  this->set_mirroring(this->info.mirroring);
}
}  // namespace nes
