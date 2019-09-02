#pragma once

#include <array>

#include "common.h"
#include "utility/snapshotable.h"

namespace nes {
class controller : public util::snapshotable {
public:
  controller() = default;

  void update_state(size_t, uint8_t);

  uint8_t read(size_t);
  void    write(bool);

  //
  // Read status without side effects
  //

  uint8_t peek(size_t) const;

  //
  // Snapshot
  //

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  bool                   strobe = false;      // Controller strobe latch
  std::array<uint8_t, 2> controller_bits = {0, 0};   // Controller shift registers
  std::array<uint8_t, 2> controller_state = {0, 0};  // Controller states
};
}  // namespace nes
