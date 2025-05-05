#pragma once

#include <array>

#include "lib/common.h"
#include "utility/snapshotable.hpp"

namespace nes {
class Controller final : public utility::Snapshotable {
public:
  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;
  auto operator=(const Controller&) -> Controller& = delete;
  auto operator=(Controller&&) -> Controller& = delete;

  static auto get() -> Controller&;

  void update_state(usize, u8);

  auto read(usize) -> u8;
  void write(bool);

  //
  // Read status without side effects
  //

  auto peek(usize) const -> u8;

  //
  // Snapshot
  //

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  Controller() = default;

  bool strobe = false;                     // Controller strobe latch
  std::array<u8, 2> controller_bits = {};  // Controller shift registers
  std::array<u8, 2> controller_state = {}; // Controller states
};
} // namespace nes
