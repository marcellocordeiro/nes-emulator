#include "ppumask.hpp"

namespace nes::types::ppu {

auto Ppumask::grayscale() const -> bool {
  return ((raw >> 0) & 1) != 0;
}

auto Ppumask::bg_left() const -> bool {
  return ((raw >> 1) & 1) != 0;
}

auto Ppumask::spr_left() const -> bool {
  return ((raw >> 2) & 1) != 0;
}

auto Ppumask::show_bg() const -> bool {
  return ((raw >> 3) & 1) != 0;
}

auto Ppumask::show_spr() const -> bool {
  return ((raw >> 4) & 1) != 0;
}

auto Ppumask::rgb() const -> u8 {
  return (raw >> 5) & 0b111;
}
} // namespace nes::types::ppu
