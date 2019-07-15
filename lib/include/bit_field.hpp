#pragma once

#include <cstddef>
#include <limits>
#include <type_traits>

namespace lib {
template <typename T, std::size_t position, std::size_t bits> struct bit_field {
public:
  using value_type = T;

  constexpr bit_field() = default;

  constexpr value_type get_mask() const
  {
    return (std::numeric_limits<value_type>::max() >>
            (8 * sizeof(value_type) - bits))
           << position;
  }

  constexpr value_type get() const
  {
    return (data & get_mask()) >> position;
  }

  constexpr void set(value_type value)
  {
    data = (data & ~get_mask()) | ((value << position) & get_mask());
  }

  constexpr operator value_type() const
  {
    return get();
  }

  constexpr bit_field& operator=(value_type val)
  {
    this->set(val);
    return *this;
  }

  constexpr bit_field& operator+=(value_type rhs)
  {
    *this = *this + rhs;
    return *this;
  }

  constexpr bit_field& operator++()
  {
    *this = *this + 1;
    return *this;
  }

  constexpr bit_field& operator++(int)
  {
    *this = *this + 1;
    return *this;
  }

  constexpr bit_field& operator^=(value_type rhs)
  {
    *this = *this ^ rhs;
    return *this;
  }

private:
  value_type data;

  static_assert(
      std::is_unsigned<value_type>::value,
      "Value type is not an unsigned integer");
};
}  // namespace lib
