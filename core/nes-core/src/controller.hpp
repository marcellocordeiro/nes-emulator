#pragma once

#include <array>

#include "lib/common.hpp"

namespace nes {
class Controller final {
public:
  [[nodiscard]] static auto get() -> Controller&;

  void update_state(usize port, u8 state);

  [[nodiscard]] auto read(usize port) -> u8;
  void write(bool signal);

  //
  // Read status without side effects
  //

  [[nodiscard]] auto peek(usize port) const -> u8;

private:
  Controller() = default;

  bool strobe = false;                     // Controller strobe latch
  std::array<u8, 2> controller_bits = {};  // Controller shift registers
  std::array<u8, 2> controller_state = {}; // Controller states
};
} // namespace nes
