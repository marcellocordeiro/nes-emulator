#pragma once

#include <cstddef>

namespace lib {
template <typename T>
struct BitfieldHelper {
  template <std::size_t Mask, std::size_t Position>
  [[nodiscard]] constexpr auto get() const -> T {
    return (raw & Mask) >> Position;
  }

  template <std::size_t Mask, std::size_t Position>
  constexpr void set(const T value) {
    raw = (raw & ~Mask) | ((value << Position) & Mask);
  }

  T raw = 0;
};
} // namespace lib
