#pragma once

#include "lib/common.hpp"
#include "lib/bitfield_helper.hpp"

namespace nes::types::ppu {
// Loopy's VRAM address
struct LoopyAddr: lib::BitfieldHelper<u16> {
  [[nodiscard]] auto coarse_x() const -> u16;
  [[nodiscard]] auto coarse_y() const -> u16;
  [[nodiscard]] auto nt() const -> u16; // Nametable
  [[nodiscard]] auto fine_y() const -> u16;

  [[nodiscard]] auto addr() const -> u16;
  [[nodiscard]] auto addr_low() const -> u16;
  [[nodiscard]] auto addr_high() const -> u16;

  void set_coarse_x(u16 value);
  void set_coarse_y(u16 value);
  void set_nt(u16 value);
  void set_fine_y(u16 value);
  void set_addr_low(u16 value);
  void set_addr_high(u16 value);
  void set_addr(u16 value);
};
} // namespace nes::types::ppu
