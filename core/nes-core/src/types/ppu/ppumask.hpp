#pragma once

#include "lib/bitfield.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
template <std::size_t Position, std::size_t Bits>
using BitfieldU8 = lib::Bitfield<std::uint8_t, Position, Bits>;

template <std::size_t Position, std::size_t Bits>
using BitfieldU16 = lib::Bitfield<std::uint16_t, Position, Bits>;

// PPUMASK ($2001) register
/*union ppumask {
  u8 raw = 0;

  BitfieldU8<0, 1> grayscale; // Grayscale
  BitfieldU8<1, 1> bg_left;   // Show background in the first column (8 pixels)
  BitfieldU8<2, 1> spr_left;  // Show sprite in the first column (8 pixels)
  BitfieldU8<3, 1> show_bg;   // Show background
  BitfieldU8<4, 1> show_spr;  // Show sprites
  BitfieldU8<5, 3> rgb;       // Palette picker (colour emphasis)
};*/

// PPUMASK ($2001) register
struct Ppumask {
  u8 raw = 0;

  explicit operator u8() const {
    return raw;
  }

  /*void set_raw(u8 value) {
    raw = value;
  }

  [[nodiscard]] auto get_raw() const -> u8 {
    return raw;
  }

  void set_grayscale(bool value) {
    raw = (raw & ~(1 << 0)) | (value << 0);
  }

  void set_bg_left(bool value) {
    raw = (raw & ~(1 << 1)) | (value << 1);
  }

  void set_spr_left(bool value) {
    raw = (raw & ~(1 << 2)) | (value << 2);
  }

  void set_show_bg(bool value) {
    raw = (raw & ~(1 << 3)) | (value << 3);
  }

  void set_show_spr(bool value) {
    raw = (raw & ~(1 << 4)) | (value << 4);
  }

  void set_rgb(u8 value) {
    raw = (raw & ~(0b111 << 5)) | ((value & 0b111) << 5);
  }*/

  [[nodiscard]] auto grayscale() const -> bool;

  [[nodiscard]] auto bg_left() const -> bool;

  [[nodiscard]] auto spr_left() const -> bool;

  [[nodiscard]] auto show_bg() const -> bool;

  [[nodiscard]] auto show_spr() const -> bool;

  [[nodiscard]] auto rgb() const -> u8;
};


}  // namespace nes::types::ppu
