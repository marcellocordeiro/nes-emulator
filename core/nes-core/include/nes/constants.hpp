#pragma once

#include <string_view>

#include "lib/common.hpp"

namespace nes {
inline constexpr u32 SCREEN_WIDTH = 256;
inline constexpr u32 SCREEN_HEIGHT = 240;
inline constexpr u32 FRAMEBUFFER_SIZE = (SCREEN_WIDTH * SCREEN_HEIGHT) * sizeof(u32);

inline constexpr auto TITLE = std::string_view("nes-emulator");
} // namespace nes
