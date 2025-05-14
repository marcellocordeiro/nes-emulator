#pragma once

#include "lib/common.hpp"

namespace nes::types::ppu {
// PPUCTRL ($2000) register
struct Ppuctrl {
  u8 raw = 0;

  explicit operator u8() const {
    return raw;
  }

  // Base nametable addr [$2000, $2400, $2800, $2C00]
  [[nodiscard]] auto nt() const -> u8;

  // Address increment [1, 32]
  [[nodiscard]] auto addr_inc() const -> bool;

  // Sprite pattern table [$0000, $1000]
  [[nodiscard]] auto spr_table() const -> bool;

  // Background pattern table [$0000, $1000]
  [[nodiscard]] auto bg_table() const -> bool;

  // Sprite size [8x8, 8x16]
  [[nodiscard]] auto spr_size() const -> bool;

  // Enable NMI on the next VBlank
  [[nodiscard]] auto nmi() const -> bool;

  // auto slave() const -> bool;  // PPU master/slave (unused)
};
} // namespace nes::types::ppu
