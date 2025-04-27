#pragma once

#include <concepts>
#include <limits>

namespace lib {
template <typename ValueType, size_t position, size_t bits>
  requires std::unsigned_integral<ValueType>
class BitField {
public:
  constexpr BitField() = default;

  constexpr operator ValueType() const noexcept {
    return get();
  }

  constexpr auto operator=(const BitField&) -> BitField& = delete;

  constexpr auto operator=(ValueType val) noexcept -> BitField& {
    this->set(val);
    return *this;
  }

  constexpr auto operator+=(ValueType rhs) noexcept -> BitField& {
    *this = *this + rhs;
    return *this;
  }

  constexpr auto operator-=(ValueType rhs) noexcept -> BitField& {
    *this = *this + rhs;
    return *this;
  }

  constexpr auto operator++() noexcept -> BitField& {
    *this = *this + 1;
    return *this;
  }

  constexpr auto operator--() noexcept -> BitField& {
    *this = *this - 1;
    return *this;
  }

  constexpr auto operator^=(ValueType rhs) noexcept -> BitField& {
    *this = *this ^ rhs;
    return *this;
  }

private:
  constexpr auto get() const noexcept -> ValueType {
    return (data & get_mask()) >> position;
  }

  constexpr void set(ValueType value) noexcept {
    data = (data & ~get_mask()) | ((value << position) & get_mask());
  }

  constexpr auto get_mask() const noexcept -> ValueType {
    return (std::numeric_limits<ValueType>::max() >> (8 * sizeof(ValueType) - bits)) << position;
  }

  ValueType data;
};
} // namespace lib
