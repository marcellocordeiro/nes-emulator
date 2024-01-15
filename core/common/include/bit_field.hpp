#pragma once

#include <limits>
#include <type_traits>

namespace lib {
template <typename T, std::size_t position, std::size_t bits>
class BitField {
public:
  using value_type = T;

  constexpr BitField() = default;

  constexpr operator value_type() const noexcept { return get(); }
  constexpr auto operator=(const BitField&) -> BitField& = delete;

  constexpr auto operator=(value_type val) noexcept -> BitField& {
    this->set(val);
    return *this;
  }

  constexpr auto operator+=(value_type rhs) noexcept -> BitField& {
    *this = *this + rhs;
    return *this;
  }

  constexpr auto operator-=(value_type rhs) noexcept -> BitField& {
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

  constexpr auto operator^=(value_type rhs) noexcept -> BitField& {
    *this = *this ^ rhs;
    return *this;
  }

private:
  constexpr auto get() const noexcept -> value_type {
    return (data & get_mask()) >> position;
  }

  constexpr void set(value_type value) noexcept {
    data = (data & ~get_mask()) | ((value << position) & get_mask());
  }

  constexpr auto get_mask() const noexcept -> value_type {
    return (std::numeric_limits<value_type>::max()
            >> (8 * sizeof(value_type) - bits))
           << position;
  }

  value_type data;

  static_assert(
    std::is_unsigned_v<value_type>, "Value type is not an unsigned integer"
  );
};
}  // namespace lib
