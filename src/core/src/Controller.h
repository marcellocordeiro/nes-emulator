#pragma once

#include <array>

#include <common.h>
#include "Utility/Snapshotable.hpp"

namespace nes {
class Controller final : public Utility::Snapshotable {
public:
  Controller(const Controller&) = delete;
  Controller(Controller&&)      = delete;
  auto operator=(const Controller&) -> Controller& = delete;
  auto operator=(Controller&&) -> Controller& = delete;

  static auto get() -> Controller&;

  void update_state(size_t, uint8_t);

  auto read(size_t) -> uint8_t;
  void    write(bool);

  //
  // Read status without side effects
  //

  auto peek(size_t) const -> uint8_t;

  //
  // Snapshot
  //

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  Controller() = default;

  bool                   strobe           = false;  // Controller strobe latch
  std::array<uint8_t, 2> controller_bits  = {};     // Controller shift registers
  std::array<uint8_t, 2> controller_state = {};     // Controller states
};
}  // namespace nes
