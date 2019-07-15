#include "mappers/mapper2.h"

namespace nes {
mapper2::mapper2(
    const nes::cartridge_info& info_in,
    std::vector<uint8_t>&&     prg_in,
    std::vector<uint8_t>&&     chr_in)
  : mapper{info_in, std::move(prg_in), std::move(chr_in)}
{}

void mapper2::reset()
{
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, -1);
  set_chr_map<8>(0, 0);

  this->set_mirroring(this->info.mirroring);
}

void mapper2::prg_write(const uint16_t addr, const uint8_t value)
{
  if (addr >= 0x8000) {
    set_prg_map<16>(0, value);
  } else {
    // Error
  }
}
}  // namespace nes
