#include "ppustatus.hpp"

#include "lib/common.hpp"

namespace nes::types::ppu {
auto Ppustatus::spr_overflow() const -> bool {
  constexpr auto mask = 0b0010'0000;
  constexpr auto position = 5;

  return get(mask, position) != 0;
}

void Ppustatus::set_spr_overflow(const bool value) {
  constexpr auto mask = 0b0010'0000;
  constexpr auto position = 5;

  set(mask, position, static_cast<u8>(value));
}

auto Ppustatus::spr0_hit() const -> bool {
  constexpr auto mask = 0b0100'0000;
  constexpr auto position = 6;

  return get(mask, position) != 0;
}

void Ppustatus::set_spr0_hit(const bool value) {
  constexpr auto mask = 0b0100'0000;
  constexpr auto position = 6;

  set(mask, position, static_cast<u8>(value));
}

auto Ppustatus::vblank() const -> bool {
  constexpr auto mask = 0b1000'0000;
  constexpr auto position = 7;

  return get(mask, position) != 0;
}

void Ppustatus::set_vblank(const bool value) {
  constexpr auto mask = 0b1000'0000;
  constexpr auto position = 7;

  set(mask, position, static_cast<u8>(value));
}
} // namespace nes::types::ppu
