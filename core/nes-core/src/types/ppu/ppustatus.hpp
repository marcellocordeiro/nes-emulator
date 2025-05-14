#pragma once

#include "lib/bitfield.hpp"
#include "lib/common.hpp"

namespace nes::types::ppu {
template <std::size_t Position, std::size_t Bits>
using BitfieldU8 = lib::Bitfield<std::uint8_t, Position, Bits>;

template <std::size_t Position, std::size_t Bits>
using BitfieldU16 = lib::Bitfield<std::uint16_t, Position, Bits>;

// PPUSTATUS ($2002) register
/*union ppustatus {
  u8 raw = 0;

  BitfieldU8<5, 1> spr_overflow; // Sprite overflow
  BitfieldU8<6, 1> spr0_hit;     // Sprite 0 hit
  BitfieldU8<7, 1> vblank;       // In VBlank?
};
*/

struct Ppustatus {
  u8 raw = 0;

  [[nodiscard]] auto spr_overflow() const -> bool;

  [[nodiscard]] auto spr0_hit() const -> bool;

  [[nodiscard]] auto vblank() const -> bool;

  void set_spr_overflow(bool value);

  void set_spr0_hit(bool value);

  void set_vblank(bool value);
};

} // namespace nes::types::ppu
