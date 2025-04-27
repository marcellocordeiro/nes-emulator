#pragma once

#include <cstddef>
#include <cstdint>

using std::int8_t;
using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;

#ifdef _DEBUG
constexpr auto DEBUG_MODE = true;
#else
constexpr auto DEBUG_MODE = false;
#endif

#define UNUSED(x) (void)(x)
