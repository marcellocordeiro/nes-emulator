#pragma once

#include <compare>
#include <concepts>

#include "concepts/binary_ops.hpp"

#define DISABLE_PRIMITIVE_INTEGERS

namespace lib {
template <std::integral Type>
struct Integer {
  using value_type = Type;

  constexpr Integer() = default;

  constexpr Integer(value_type value) noexcept : value_{value} {}

  [[nodiscard]] constexpr auto get() const noexcept -> value_type {
    return value_;
  }

  static constexpr auto from(const std::integral auto value) -> Integer {
    return {static_cast<value_type>(value)};
  }

  template <std::integral T>
  static constexpr auto from(const Integer<T> value) -> Integer {
    return from(value.get());
  }

  constexpr explicit operator value_type() const noexcept {
    return value_;
  }

  constexpr auto operator=(const Integer& rhs) noexcept -> Integer& = default;

  constexpr Integer(const Integer& other) noexcept : value_{other.value_} {}

  constexpr auto add_signed(const matches_signed_type<value_type> auto value) const noexcept -> Integer {
    return from(value_ + value.get());
  }

  constexpr auto wrapping_add_signed(const std::integral auto value) const noexcept -> Integer {
    return from(value_ + value);
  }

  /// Comparisons incl == and !=
  friend constexpr auto operator<=>(const Integer& lhs, const Integer& rhs) noexcept = default;

#pragma region with other wrapped integers

  /// auto result = x + y
  friend constexpr auto operator+(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ + rhs.value_);
  }

  /// auto result = x - y
  friend constexpr auto operator-(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ - rhs.value_);
  }

  /// auto result = x * y
  friend constexpr auto operator*(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ * rhs.value_);
  }

  /// auto result = x / y
  friend constexpr auto operator/(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ / rhs.value_);
  }

  /// auto result = x % y
  friend constexpr auto operator%(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ % rhs.value_);
  }

#pragma endregion

  /// x += y
  constexpr auto operator+=(const Integer rhs) noexcept -> Integer& {
    value_ = from(value_ + rhs.value_);
    return *this;
  }

  /// x -= y
  constexpr auto operator-=(const Integer rhs) noexcept -> Integer& {
    value_ = from(value_ - rhs.value_);
    return *this;
  }

  /// x *= y
  constexpr auto operator*=(const Integer rhs) noexcept -> Integer& {
    value_ = from(value_ * rhs.value_);
    return *this;
  }

  /// x /= y
  constexpr auto operator/=(const Integer& rhs) noexcept -> Integer& {
    value_ = from(value_ / rhs.value_);
    return *this;
  }

  /// x %= y
  constexpr auto operator%=(const Integer rhs) noexcept -> Integer& {
    value_ = from(value_ % rhs.value_);
    return *this;
  }

#pragma region Prefix inc/dec

  /// ++x
  constexpr auto operator++() noexcept -> Integer& {
    ++value_;
    return *this;
  }

  /// --x
  constexpr auto operator--() noexcept -> Integer& {
    --value_;
    return *this;
  }

#pragma endregion

#pragma region Postfix inc/dec

  /// x++
  constexpr auto operator++(int) noexcept -> Integer {
    return from(value_++);
  }

  /// x--
  constexpr auto operator--(int) noexcept -> Integer {
    return from(value_--);
  }

#pragma endregion

#pragma region bit

  constexpr auto operator~() const noexcept -> Integer {
    return from(~value_);
  }

  friend constexpr auto operator&(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ & rhs.value_);
  }

  friend constexpr auto operator|(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ | rhs.value_);
  }

  friend constexpr auto operator^(const Integer lhs, const Integer rhs) noexcept -> Integer {
    return from(lhs.value_ ^ rhs.value_);
  }

  constexpr auto operator&=(const Integer rhs) noexcept -> Integer& {
    value_ &= rhs.value_;
    return *this;
  }

  constexpr auto operator|=(const Integer rhs) noexcept -> Integer& {
    value_ |= rhs.value_;
    return *this;
  }

  constexpr auto operator^=(const Integer rhs) noexcept -> Integer& {
    value_ ^= rhs.value_;
    return *this;
  }

#pragma endregion

  // Shifts
  // Any rhs type is fine

  /// auto result = x << y;
  constexpr auto operator<<(const std::integral auto rhs) const noexcept -> Integer {
    return from(value_ << rhs);
  }

  constexpr auto operator<<(const Integer rhs) const noexcept -> Integer {
    return from(value_ << rhs.value_);
  }

  /// auto result = x >> y;
  constexpr auto operator>>(const std::integral auto rhs) const noexcept -> Integer {
    return from(value_ >> rhs);
  }

  constexpr auto operator>>(const Integer rhs) const noexcept -> Integer {
    return from(value_ >> rhs.value_);
  }

  /// x <<= y;
  constexpr auto operator<<=(const std::integral auto rhs) noexcept -> Integer& {
    value_ <<= rhs;
    return *this;
  }

  /// x >>= y;
  constexpr auto operator>>=(const std::integral auto rhs) noexcept -> Integer& {
    value_ >>= rhs;
    return *this;
  }

private:
  value_type value_{};
};
} // namespace lib

// NOLINTBEGIN
using i8 = lib::Integer<std::int8_t>;
using i16 = lib::Integer<std::int16_t>;
using i32 = lib::Integer<std::int32_t>;
using i64 = lib::Integer<std::int64_t>;
using u8 = lib::Integer<std::uint8_t>;
using u16 = lib::Integer<std::uint16_t>;
using u32 = lib::Integer<std::uint32_t>;
using u64 = lib::Integer<std::uint64_t>;
using usize = lib::Integer<std::size_t>;

// NOLINTEND

namespace lib::literals {
consteval auto operator""_i8(const unsigned long long int value) noexcept -> i8 {
  return {static_cast<std::int8_t>(value)};
}

consteval auto operator""_i16(const unsigned long long int value) noexcept -> i16 {
  return {static_cast<std::int16_t>(value)};
}

consteval auto operator""_i32(const unsigned long long int value) noexcept -> i32 {
  return {static_cast<std::int32_t>(value)};
}

consteval auto operator""_i64(const unsigned long long int value) noexcept -> i64 {
  return {static_cast<std::int64_t>(value)};
}

consteval auto operator""_u8(const unsigned long long int value) noexcept -> u8 {
  return {static_cast<std::uint8_t>(value)};
}

consteval auto operator""_u16(const unsigned long long int value) noexcept -> u16 {
  return {static_cast<std::uint16_t>(value)};
}

consteval auto operator""_u32(const unsigned long long int value) noexcept -> u32 {
  return {static_cast<std::uint32_t>(value)};
}

consteval auto operator""_u64(const unsigned long long int value) noexcept -> u64 {
  return {static_cast<std::uint64_t>(value)};
}

consteval auto operator""_usize(const unsigned long long int value) noexcept -> usize {
  return {static_cast<std::size_t>(value)};
}
} // namespace lib::literals
