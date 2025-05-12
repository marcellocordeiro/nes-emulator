#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

namespace lib {
template <typename From, typename To>
concept is_convertible_without_narrowing = std::integral<From>
  && std::integral<To>
  && std::numeric_limits<From>::min() >= std::numeric_limits<To>::min()
  && std::numeric_limits<From>::max() <= std::numeric_limits<To>::max();

template <typename Arg, typename Self>
concept matches_signed_type = std::unsigned_integral<Self> && requires(Arg a) {
  typename Arg::value_type;
} && std::is_same_v<std::make_signed_t<Self>, typename Arg::value_type>;
} // namespace lib
