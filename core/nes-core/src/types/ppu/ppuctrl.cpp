#include "ppuctrl.hpp"

namespace nes::types::ppu {
auto Ppuctrl::nt() const -> u8 {
  constexpr auto mask = 0b0000'0011;
  return raw & mask;
}

auto Ppuctrl::addr_inc() const -> bool {
  constexpr auto mask = 0b0000'0100;
  return ((raw & mask) >> 2) != 0;

}

auto Ppuctrl::spr_table() const -> bool {
  constexpr auto mask = 0b0000'1000;
  return ((raw & mask) >> 3) != 0;

}

auto Ppuctrl::bg_table() const -> bool {
  constexpr auto mask = 0b0001'0000;
  return ((raw & mask) >> 4) != 0;

}

auto Ppuctrl::spr_size() const -> bool {
  constexpr auto mask = 0b0010'0000;
  return ((raw & mask) >> 5) != 0;

}

auto Ppuctrl::nmi() const -> bool {
  constexpr auto mask = 0b1000'0000;

  return ((raw & mask) >> 7) != 0;
}
} // namespace nes::types::ppu