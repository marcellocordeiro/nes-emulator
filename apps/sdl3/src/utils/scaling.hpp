#pragma once

#include <algorithm>
#include <cmath>

#include "../sdl/sdl.hpp"
#include "lib/common.hpp"
#include "vec2.hpp"

[[nodiscard]] static auto integer_scale(const Vec2 available_size, const Vec2 texture_size) -> i32 {
  const auto width_ratio = std::max(
    static_cast<double>(available_size.width) / static_cast<double>(texture_size.width),
    1.0
  );
  const auto height_ratio = std::max(
    static_cast<double>(available_size.height) / static_cast<double>(texture_size.height),
    1.0
  );

  const auto scale = std::floor(std::clamp(width_ratio, 1.0, height_ratio));

  return static_cast<i32>(scale);
}

[[nodiscard]] static auto
integer_scale_centered_rect(const Vec2 available_size, const Vec2 texture_size) -> SDL_FRect {
  const auto scale = integer_scale(available_size, texture_size);

  const auto w = static_cast<float>(texture_size.height * scale);
  const auto h = static_cast<float>(texture_size.width * scale);

  const float x = (static_cast<float>(available_size.width) / 2) - (w / 2);
  const float y = (static_cast<float>(available_size.height) / 2) - (h / 2);

  return {.x = x, .y = y, .w = w, .h = h};
}
