#pragma once

#include <cstddef>
#include <cstdint>

using std::int8_t;
using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;

enum class OS { Windows, Linux, Unsupported };

#ifdef _WIN32
constexpr auto CURRENT_OS = OS::Windows;
#elif __linux__
constexpr auto CURRENT_OS = OS::Linux;
#else
constexpr auto CURRENT_OS = OS::Unsupported;
#endif

static_assert(CURRENT_OS != OS::Unsupported, "Unsupported OS");

#ifdef _DEBUG
constexpr auto DEBUG_MODE = true;
#else
constexpr auto DEBUG_MODE = false;
#endif

inline const char* app_path = nullptr;
