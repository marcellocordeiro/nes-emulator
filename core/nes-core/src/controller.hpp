#pragma once

#include <array>

#include "lib/common.hpp"
#include "utility/snapshotable.hpp"

namespace nes {
class Controller final: public utility::Snapshotable {
public:
  static auto get() -> Controller&;

  void update_state(usize, u8);

  auto read(usize) -> u8;
  void write(bool);

  //
  // Read status without side effects
  //

  [[nodiscard]] auto peek(usize) const -> u8;

  //
  // Snapshot
  //

  void save(std::ofstream& out) const override;
  void load(std::ifstream& in) override;

private:
  Controller() = default;

  bool strobe = false;                     // Controller strobe latch
  std::array<u8, 2> controller_bits = {};  // Controller shift registers
  std::array<u8, 2> controller_state = {}; // Controller states
};
} // namespace nes
