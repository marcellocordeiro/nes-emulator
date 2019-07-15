#pragma once

#include <array>

#include "bus.h"
#include "common.h"

namespace nes {
class controller {
public:
  void set_bus(nes::bus&);

  uint8_t read(size_t);
  void    write(bool);

private:
  nes::bus* bus = nullptr;

  bool                   strobe = false;   // Controller strobe latch
  std::array<uint8_t, 2> controller_bits;  // Controller shift registers
};
}  // namespace nes
