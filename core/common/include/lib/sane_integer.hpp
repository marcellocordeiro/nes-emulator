#pragma once

#include <compare>
#include <concepts>

#include "concepts/binary_ops.hpp"

namespace lib {
template <std::integral Type>
struct SaneInteger {
  using value_type = Type;

  constexpr SaneInteger() = default;

  constexpr SaneInteger(value_type value) noexcept : value_{value} {}

  [[nodiscard]] constexpr auto get() const noexcept -> value_type {
    return value_;
  }

  [[nodiscard]] static constexpr auto from(const std::integral auto value) -> SaneInteger {
    return {static_cast<value_type>(value)};
  }

  template <std::integral T>
  [[nodiscard]] static constexpr auto from(const SaneInteger<T> value) -> SaneInteger {
    return from(value.get());
  }

  constexpr explicit operator value_type() const noexcept {
    return value_;
  }

  constexpr auto add_signed(const matches_signed_type<value_type> auto value) const noexcept
    -> SaneInteger {
    return from(value_ + value.get());
  }

  constexpr auto wrapping_add_signed(const std::integral auto value) const noexcept -> SaneInteger {
    return from(value_ + value);
  }

  /// Comparisons incl == and !=
  friend constexpr auto operator<=>(const SaneInteger& lhs, const SaneInteger& rhs) noexcept =
    default;

#pragma region with other wrapped integers

  /// auto result = x + y
  [[nodiscard]] friend constexpr auto
  operator+(const SaneInteger lhs, const SaneInteger rhs) noexcept -> SaneInteger {
    return from(lhs.value_ + rhs.value_);
  }

  /// auto result = x - y
  [[nodiscard]] friend constexpr auto
  operator-(const SaneInteger lhs, const SaneInteger rhs) noexcept -> SaneInteger {
    return from(lhs.value_ - rhs.value_);
  }

  /// auto result = x * y
  [[nodiscard]] friend constexpr auto
  operator*(const SaneInteger lhs, const SaneInteger rhs) noexcept -> SaneInteger {
    return from(lhs.value_ * rhs.value_);
  }

  /// auto result = x / y
  [[nodiscard]] friend constexpr auto
  operator/(const SaneInteger lhs, const SaneInteger rhs) noexcept -> SaneInteger {
    return from(lhs.value_ / rhs.value_);
  }

  /// auto result = x % y
  [[nodiscard]] friend constexpr auto
  operator%(const SaneInteger lhs, const SaneInteger rhs) noexcept -> SaneInteger {
    return from(lhs.value_ % rhs.value_);
  }

#pragma endregion

  /// x += y
  [[nodiscard]] constexpr auto operator+=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ = from(value_ + rhs.value_);
    return *this;
  }

  /// x -= y
  [[nodiscard]] constexpr auto operator-=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ = from(value_ - rhs.value_);
    return *this;
  }

  /// x *= y
  [[nodiscard]] constexpr auto operator*=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ = from(value_ * rhs.value_);
    return *this;
  }

  /// x /= y
  [[nodiscard]] constexpr auto operator/=(const SaneInteger& rhs) noexcept -> SaneInteger& {
    value_ = from(value_ / rhs.value_);
    return *this;
  }

  /// x %= y
  [[nodiscard]] constexpr auto operator%=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ = from(value_ % rhs.value_);
    return *this;
  }

#pragma region Prefix inc/dec

  /// ++x
  constexpr auto operator++() noexcept -> SaneInteger& {
    ++value_;
    return *this;
  }

  /// --x
  constexpr auto operator--() noexcept -> SaneInteger& {
    --value_;
    return *this;
  }

#pragma endregion

#pragma region Postfix inc/dec

  /// x++
  constexpr auto operator++(int) noexcept -> SaneInteger {
    return from(value_++);
  }

  /// x--
  constexpr auto operator--(int) noexcept -> SaneInteger {
    return from(value_--);
  }

#pragma endregion

#pragma region bit

  constexpr auto operator~() const noexcept -> SaneInteger {
    return from(~value_);
  }

  friend constexpr auto operator&(const SaneInteger lhs, const SaneInteger rhs) noexcept
    -> SaneInteger {
    return from(lhs.value_ & rhs.value_);
  }

  friend constexpr auto operator|(const SaneInteger lhs, const SaneInteger rhs) noexcept
    -> SaneInteger {
    return from(lhs.value_ | rhs.value_);
  }

  friend constexpr auto operator^(const SaneInteger lhs, const SaneInteger rhs) noexcept
    -> SaneInteger {
    return from(lhs.value_ ^ rhs.value_);
  }

  constexpr auto operator&=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ &= rhs.value_;
    return *this;
  }

  constexpr auto operator|=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ |= rhs.value_;
    return *this;
  }

  constexpr auto operator^=(const SaneInteger rhs) noexcept -> SaneInteger& {
    value_ ^= rhs.value_;
    return *this;
  }

#pragma endregion

  // Shifts
  // Any rhs type is fine

  /// auto result = x << y;
  constexpr auto operator<<(const std::integral auto rhs) const noexcept -> SaneInteger {
    return from(value_ << rhs);
  }

  constexpr auto operator<<(const SaneInteger rhs) const noexcept -> SaneInteger {
    return from(value_ << rhs.value_);
  }

  /// auto result = x >> y;
  constexpr auto operator>>(const std::integral auto rhs) const noexcept -> SaneInteger {
    return from(value_ >> rhs);
  }

  constexpr auto operator>>(const SaneInteger rhs) const noexcept -> SaneInteger {
    return from(value_ >> rhs.value_);
  }

  /// x <<= y;
  constexpr auto operator<<=(const std::integral auto rhs) noexcept -> SaneInteger& {
    value_ <<= rhs;
    return *this;
  }

  /// x >>= y;
  constexpr auto operator>>=(const std::integral auto rhs) noexcept -> SaneInteger& {
    value_ >>= rhs;
    return *this;
  }

private:
  value_type value_{};
};
} // namespace lib

// NOLINTBEGIN
// Meant to be incrementally applied.
// Remove the _s suffix after replacing.
using i8_s = lib::SaneInteger<std::int8_t>;
using i16_s = lib::SaneInteger<std::int16_t>;
using i32_s = lib::SaneInteger<std::int32_t>;
using i64_s = lib::SaneInteger<std::int64_t>;
using u8_s = lib::SaneInteger<std::uint8_t>;
using u16_s = lib::SaneInteger<std::uint16_t>;
using u32_s = lib::SaneInteger<std::uint32_t>;
using u64_s = lib::SaneInteger<std::uint64_t>;
using usize_s = lib::SaneInteger<std::size_t>;

// NOLINTEND

namespace lib::literals {
consteval auto operator""_i8(const unsigned long long int value) noexcept -> i8_s {
  return {static_cast<std::int8_t>(value)};
}

consteval auto operator""_i16(const unsigned long long int value) noexcept -> i16_s {
  return {static_cast<std::int16_t>(value)};
}

consteval auto operator""_i32(const unsigned long long int value) noexcept -> i32_s {
  return {static_cast<std::int32_t>(value)};
}

consteval auto operator""_i64(const unsigned long long int value) noexcept -> i64_s {
  return {static_cast<std::int64_t>(value)};
}

consteval auto operator""_u8(const unsigned long long int value) noexcept -> u8_s {
  return {static_cast<std::uint8_t>(value)};
}

consteval auto operator""_u16(const unsigned long long int value) noexcept -> u16_s {
  return {static_cast<std::uint16_t>(value)};
}

consteval auto operator""_u32(const unsigned long long int value) noexcept -> u32_s {
  return {static_cast<std::uint32_t>(value)};
}

consteval auto operator""_u64(const unsigned long long int value) noexcept -> u64_s {
  return {static_cast<std::uint64_t>(value)};
}

consteval auto operator""_usize(const unsigned long long int value) noexcept -> usize_s {
  return {static_cast<std::size_t>(value)};
}
} // namespace lib::literals
