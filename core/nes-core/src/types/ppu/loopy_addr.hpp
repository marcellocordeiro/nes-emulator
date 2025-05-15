#pragma once

#include "lib/bitfield_helper.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
// Loopy's VRAM address
struct LoopyAddr: lib::BitfieldHelper<u16> {
  [[nodiscard]] auto coarse_x() const -> u16;
  void set_coarse_x(u16 value);

  [[nodiscard]] auto coarse_y() const -> u16;
  void set_coarse_y(u16 value);

  // Nametable
  [[nodiscard]] auto nt() const -> u16;
  void set_nt(u16 value);

  [[nodiscard]] auto fine_y() const -> u16;
  void set_fine_y(u16 value);

  [[nodiscard]] auto addr() const -> u16;
  void set_addr(u16 value);

  [[nodiscard]] auto addr_low() const -> u16;
  void set_addr_low(u16 value);

  [[nodiscard]] auto addr_high() const -> u16;
  void set_addr_high(u16 value);
};
} // namespace nes::types::ppu
