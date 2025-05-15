#pragma once

#include "lib/bitfield_helper.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
// PPUSTATUS ($2002) register
struct Ppustatus: lib::BitfieldHelper<u8> {
  // Sprite overflow
  [[nodiscard]] auto spr_overflow() const -> bool;
  void set_spr_overflow(bool value);

  // Sprite 0 hit
  [[nodiscard]] auto spr0_hit() const -> bool;
  void set_spr0_hit(bool value);

  // In VBlank?
  [[nodiscard]] auto vblank() const -> bool;
  void set_vblank(bool value);
};
} // namespace nes::types::ppu
