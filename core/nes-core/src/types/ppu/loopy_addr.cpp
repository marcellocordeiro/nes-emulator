#include "loopy_addr.hpp"

#include "lib/common.hpp"

namespace nes::types::ppu {
auto LoopyAddr::coarse_x() const -> u16 {
  constexpr auto position = 0;
  constexpr auto mask = 0b0001'1111;

  return get(mask, position);
}

void LoopyAddr::set_coarse_x(const u16 value) {
  constexpr auto position = 0;
  constexpr auto mask = 0b0001'1111;

  set(mask, position, value);
}

auto LoopyAddr::coarse_y() const -> u16 {
  constexpr auto position = 5;
  constexpr auto mask = 0b0000'0011'1110'0000;

  return get(mask, position);
}

void LoopyAddr::set_coarse_y(const u16 value) {
  constexpr auto position = 5;
  constexpr auto mask = 0b0000'0011'1110'0000;

  set(mask, position, value);
}

auto LoopyAddr::nt() const -> u16 {
  constexpr auto position = 10;
  constexpr auto mask = 0b0000'1100'0000'0000;

  return get(mask, position);
}

void LoopyAddr::set_nt(const u16 value) {
  constexpr auto position = 10;
  constexpr auto mask = 0b0000'1100'0000'0000;

  set(mask, position, value);
}

auto LoopyAddr::fine_y() const -> u16 {
  constexpr auto position = 12;
  constexpr auto mask = 0b0111'0000'0000'0000;

  return get(mask, position);
}

void LoopyAddr::set_fine_y(const u16 value) {
  constexpr auto position = 12;
  constexpr auto mask = 0b0111'0000'0000'0000;

  set(mask, position, value);
}

auto LoopyAddr::addr() const -> u16 {
  constexpr auto position = 0;
  constexpr auto mask = 0b0011'1111'1111'1111;

  return get(mask, position);
}

void LoopyAddr::set_addr(const u16 value) {
  constexpr auto position = 0;
  constexpr auto mask = 0b0011'1111'1111'1111;

  set(mask, position, value);
}

auto LoopyAddr::addr_low() const -> u16 {
  constexpr auto position = 0;
  constexpr auto mask = 0b1111'1111;

  return get(mask, position);
}

void LoopyAddr::set_addr_low(const u16 value) {
  constexpr auto position = 0;
  constexpr auto mask = 0b1111'1111;

  set(mask, position, value);
}

auto LoopyAddr::addr_high() const -> u16 {
  constexpr auto position = 8;
  constexpr auto mask = 0b0111'1111'0000'0000;

  return get(mask, position);
}

void LoopyAddr::set_addr_high(const u16 value) {
  constexpr auto position = 8;
  constexpr auto mask = 0b0111'1111'0000'0000;

  set(mask, position, value);
}
} // namespace nes::types::ppu
