#pragma once

#include <array>

#include <common.h>
#include "Utility/Snapshotable.hpp"

namespace nes {
class controller : public util::snapshotable {
public:
  controller(const controller&) = delete;
  controller(controller&&)      = delete;
  controller& operator=(const controller&) = delete;
  controller& operator=(controller&&) = delete;

  static controller& get();

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
  controller() = default;

  bool                   strobe           = false;  // Controller strobe latch
  std::array<uint8_t, 2> controller_bits  = {};  // Controller shift registers
  std::array<uint8_t, 2> controller_state = {};  // Controller states
};
}  // namespace nes