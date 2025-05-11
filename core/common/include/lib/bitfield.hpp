#pragma once

#include <concepts>
#include <limits>

#include "common.hpp"

namespace lib {
template <typename ValueType, usize Position, usize Bits>
  requires std::unsigned_integral<ValueType>
class Bitfield {
public:
  constexpr Bitfield() = default;

  constexpr operator ValueType() const noexcept {
    return get();
  }

  constexpr auto operator=(const Bitfield&) -> Bitfield& = delete;

  constexpr auto operator=(ValueType val) noexcept -> Bitfield& {
    this->set(val);
    return *this;
  }

  constexpr auto operator+=(ValueType rhs) noexcept -> Bitfield& {
    *this = *this + rhs;
    return *this;
  }

  constexpr auto operator-=(ValueType rhs) noexcept -> Bitfield& {
    *this = *this + rhs;
    return *this;
  }

  constexpr auto operator++() noexcept -> Bitfield& {
    *this = *this + 1;
    return *this;
  }

  constexpr auto operator--() noexcept -> Bitfield& {
    *this = *this - 1;
    return *this;
  }

  constexpr auto operator^=(ValueType rhs) noexcept -> Bitfield& {
    *this = *this ^ rhs;
    return *this;
  }

private:
  constexpr auto get() const noexcept -> ValueType {
    return (data & get_mask()) >> Position;
  }

  constexpr void set(ValueType value) noexcept {
    data = (data & ~get_mask()) | ((value << Position) & get_mask());
  }

  constexpr auto get_mask() const noexcept -> ValueType {
    return (std::numeric_limits<ValueType>::max() >> (8 * sizeof(ValueType) - Bits)) << Position;
  }

  ValueType data;
};
} // namespace lib
