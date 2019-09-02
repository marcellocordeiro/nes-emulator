#include "mappers/mapper0.h"

namespace nes {
mapper0::mapper0(cartridge& cart_ref) : base_mapper(cart_ref) {}

void mapper0::reset()
{
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, 1);
  set_chr_map<8>(0, 0);
}
}  // namespace nes
