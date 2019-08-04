#pragma once

#include <array>

#include "common.h"
#include "types/forward_decl.h"
#include "utility/snapshotable.h"

namespace nes {
class controller : public util::snapshotable {
public:
  controller(nes::emulator&);

  uint8_t read(size_t);
  void    write(bool);

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  nes::emulator& emulator;

  bool                   strobe = false;   // Controller strobe latch
  std::array<uint8_t, 2> controller_bits{};  // Controller shift registers
};
}  // namespace nes
