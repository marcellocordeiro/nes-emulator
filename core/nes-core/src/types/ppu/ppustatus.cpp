#include "ppustatus.hpp"

namespace nes::types::ppu {

auto Ppustatus::spr_overflow() const -> bool {
  return ((raw >> 5) & 1) != 0;
}

auto Ppustatus::spr0_hit() const -> bool {
  return ((raw >> 6) & 1) != 0;
}

auto Ppustatus::vblank() const -> bool {
  return ((raw >> 7) & 1) != 0;
}

void Ppustatus::set_spr_overflow(const bool value) {
  raw = (raw & ~(1 << 5)) | (static_cast<u8>(value) << 5);
}

void Ppustatus::set_spr0_hit(const bool value) {
  raw = (raw & ~(1 << 6)) | (static_cast<u8>(value) << 6);
}

void Ppustatus::set_vblank(const bool value) {
  raw = (raw & ~(1 << 7)) | (static_cast<u8>(value) << 7);
}
} // namespace nes::types::ppu
