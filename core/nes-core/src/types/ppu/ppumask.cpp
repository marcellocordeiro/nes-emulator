#include "ppumask.hpp"

#include "lib/common.hpp"

namespace nes::types::ppu {
auto Ppumask::grayscale() const -> bool {
  constexpr auto mask = 0b0000'0001;
  constexpr auto position = 0;

  return get(mask, position) != 0;
}

auto Ppumask::bg_left() const -> bool {
  constexpr auto mask = 0b0000'0010;
  constexpr auto position = 1;

  return get(mask, position) != 0;
}

auto Ppumask::spr_left() const -> bool {
  constexpr auto mask = 0b0000'0100;
  constexpr auto position = 2;

  return get(mask, position) != 0;
}

auto Ppumask::show_bg() const -> bool {
  constexpr auto mask = 0b0000'1000;
  constexpr auto position = 3;

  return get(mask, position) != 0;
}

auto Ppumask::show_spr() const -> bool {
  constexpr auto mask = 0b0001'0000;
  constexpr auto position = 4;

  return get(mask, position) != 0;
}

auto Ppumask::rgb() const -> u8 {
  constexpr auto mask = 0b1110'0000;
  constexpr auto position = 5;

  return get(mask, position);
}
} // namespace nes::types::ppu
