#pragma once

#include <array>

#include "common.h"
#include "types/forward_decl.h"

namespace nes {
class controller {
public:
  controller(nes::io&);

  uint8_t read(size_t);
  void    write(bool);

private:
  nes::io& io;

  bool                   strobe = false;   // Controller strobe latch
  std::array<uint8_t, 2> controller_bits;  // Controller shift registers
};
}  // namespace nes
