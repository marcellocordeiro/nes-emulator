#pragma once

#include "lib/bitfield_helper.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
// PPUMASK ($2001) register
struct Ppumask: lib::BitfieldHelper<u8> {
  // Grayscale
  [[nodiscard]] auto grayscale() const -> bool;

  // Show background in the first column (8 pixels)
  [[nodiscard]] auto bg_left() const -> bool;

  // Show sprite in the first column (8 pixels)
  [[nodiscard]] auto spr_left() const -> bool;

  // Show background
  [[nodiscard]] auto show_bg() const -> bool;

  // Show sprites
  [[nodiscard]] auto show_spr() const -> bool;

  // Palette picker (colour emphasis)
  [[nodiscard]] auto rgb() const -> u8;
};
} // namespace nes::types::ppu
