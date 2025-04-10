#include "mapper_0.h"

namespace nes {
void Mapper0::reset() {
  set_prg_map<16>(0, 0);
  set_prg_map<16>(1, 1);
  set_chr_map<8>(0, 0);
}
} // namespace nes
