#pragma once

#include <limits>
#include <type_traits>

namespace lib {
template <typename T, std::size_t position, std::size_t bits> class bit_field {
public:
  using value_type = T;

  constexpr bit_field() = default;

  constexpr            operator value_type() const noexcept { return get(); }
  constexpr bit_field& operator=(const bit_field&) = delete;

  constexpr bit_field& operator=(value_type val) noexcept
  {
    this->set(val);
    return *this;
  }

  constexpr bit_field& operator+=(value_type rhs) noexcept
  {
    *this = *this + rhs;
    return *this;
  }

  constexpr bit_field& operator++() noexcept
  {
    *this = *this + 1;
    return *this;
  }

  constexpr bit_field& operator^=(value_type rhs) noexcept
  {
    *this = *this ^ rhs;
    return *this;
  }

private:
  constexpr value_type get() const noexcept
  {
    return (data & get_mask()) >> position;
  }

  constexpr void set(value_type value) noexcept
  {
    data = (data & ~get_mask()) | ((value << position) & get_mask());
  }

  constexpr value_type get_mask() const noexcept
  {
    return (std::numeric_limits<value_type>::max() >>
            (8 * sizeof(value_type) - bits))
           << position;
  }

  value_type data;

  static_assert(std::is_unsigned_v<value_type>,
                "Value type is not an unsigned integer");
};
}  // namespace lib
