#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>

// #include "integer.hpp" // IWYU pragma: export

// NOLINTBEGIN
#ifndef DISABLE_PRIMITIVE_INTEGERS
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using usize = std::size_t;
#endif
// NOLINTEND

#ifndef NDEBUG
#define DEBUG
constexpr auto DEBUG_MODE = true;
#else
constexpr auto DEBUG_MODE = false;
#endif

#define UNUSED(x) (void)(x)

[[noreturn]] inline void unreachable() {
  std::terminate();
}
