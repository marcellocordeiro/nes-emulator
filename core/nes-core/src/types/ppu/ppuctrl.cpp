#include "ppuctrl.hpp"

#include "lib/common.hpp"

namespace nes::types::ppu {
auto Ppuctrl::nt() const -> u8 {
  constexpr auto mask = 0b0000'0011;
  constexpr auto position = 0;

  return get(mask, position);
}

auto Ppuctrl::addr_inc() const -> bool {
  constexpr auto mask = 0b0000'0100;
  constexpr auto position = 2;

  return get(mask, position) != 0;
}

auto Ppuctrl::spr_table() const -> bool {
  constexpr auto mask = 0b0000'1000;
  constexpr auto position = 3;

  return get(mask, position) != 0;
}

auto Ppuctrl::bg_table() const -> bool {
  constexpr auto mask = 0b0001'0000;
  constexpr auto position = 4;

  return get(mask, position) != 0;
}

auto Ppuctrl::spr_size() const -> bool {
  constexpr auto mask = 0b0010'0000;
  constexpr auto position = 5;

  return get(mask, position) != 0;
}

auto Ppuctrl::nmi() const -> bool {
  constexpr auto mask = 0b1000'0000;
  constexpr auto position = 7;

  return get(mask, position) != 0;
}
} // namespace nes::types::ppu
