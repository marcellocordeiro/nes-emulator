#pragma once

#include <cstring>
#include <type_traits>

namespace lib {
template <typename To, typename From> To bit_cast(const From& from) noexcept
{
  static_assert(
      sizeof(To) == sizeof(From), "bit_cast requires both sizes to be equal");
  static_assert(
      std::is_trivially_copyable<From>::value,
      "bit_cast requires the source type to be trivially copyable");
  static_assert(
      std::is_trivially_copyable<To>::value,
      "bit_cast requires the destination type to be trivially copyable");

  To result;
  std::memcpy(&result, &from, sizeof(From));

  return result;
}
}  // namespace lib
