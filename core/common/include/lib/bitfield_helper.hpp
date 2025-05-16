#pragma once

#include <cstddef>

namespace lib {
template <typename T>
struct BitfieldHelper {
  [[nodiscard]] constexpr auto get(T mask, std::size_t position) const -> T {
    return (raw & mask) >> position;
  }

  constexpr void set(T mask, std::size_t position, const T value) {
    raw = (raw & ~mask) | ((value << position) & mask);
  }

  T raw = 0;
};
} // namespace lib
