#include "loopy_addr.hpp"

namespace coarse_x {
constexpr auto POSITION = 0;
constexpr auto MASK = u16{0b1'1111 << POSITION};
} // namespace coarse_x

namespace coarse_y {
constexpr auto POSITION = 5;
constexpr auto MASK = u16{0b1'1111 << POSITION};
} // namespace coarse_y

namespace nt {
constexpr auto POSITION = 10;
constexpr auto MASK = u16{0b11 << POSITION};
} // namespace nt

namespace fine_y {
constexpr auto POSITION = 12;
constexpr auto MASK = u16{0b111 << POSITION};
} // namespace fine_y

namespace addr {
constexpr auto POSITION = 0;
constexpr auto MASK = u16{0b0011'1111'1111'1111 << POSITION};
} // namespace addr

namespace addr_low {
constexpr auto POSITION = 0;
constexpr auto MASK = u16{0b1111'1111 << POSITION};
} // namespace addr_low

namespace addr_high {
constexpr auto POSITION = 8;
constexpr auto MASK = u16{0b0111'1111 << POSITION};
} // namespace addr_high

namespace nes::types::ppu {
auto LoopyAddr::coarse_x() const -> u16 {
  using ::coarse_x::MASK;
  using ::coarse_x::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::coarse_y() const -> u16 {
  using ::coarse_y::MASK;
  using ::coarse_y::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::nt() const -> u16 {
  using ::nt::MASK;
  using ::nt::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::fine_y() const -> u16 {
  using ::fine_y::MASK;
  using ::fine_y::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::addr() const -> u16 {
  using ::addr::MASK;
  using ::addr::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::addr_low() const -> u16 {
  using ::addr_low::MASK;
  using ::addr_low::POSITION;

  return get<MASK, POSITION>();
}

auto LoopyAddr::addr_high() const -> u16 {
  using ::addr_high::MASK;
  using ::addr_high::POSITION;

  return get<MASK, POSITION>();
}

void LoopyAddr::set_coarse_x(const u16 value) {
  using ::coarse_x::MASK;
  using ::coarse_x::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_coarse_y(const u16 value) {
  using ::coarse_y::MASK;
  using ::coarse_y::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_nt(const u16 value) {
  using ::nt::MASK;
  using ::nt::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_fine_y(const u16 value) {
  using ::fine_y::MASK;
  using ::fine_y::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_addr(const u16 value) {
  using ::addr::MASK;
  using ::addr::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_addr_low(const u16 value) {
  using ::addr_low::MASK;
  using ::addr_low::POSITION;

  set<MASK, POSITION>(value);
}

void LoopyAddr::set_addr_high(const u16 value) {
  using ::addr_high::MASK;
  using ::addr_high::POSITION;

  set<MASK, POSITION>(value);
}
} // namespace nes::types::ppu
