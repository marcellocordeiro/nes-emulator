#pragma once

#include <algorithm>
#include <cmath>

#include "../sdl/sdl.hpp"
#include "lib/common.hpp"
#include "vec2.hpp"

static auto integer_scale(Vec2 availableSize, Vec2 textureSize) -> i32 {
  auto widthRatio = std::max(
    static_cast<double>(availableSize.width) / static_cast<double>(textureSize.width),
    1.0
  );
  auto heightRatio = std::max(
    static_cast<double>(availableSize.height) / static_cast<double>(textureSize.height),
    1.0
  );

  auto scale = std::floor(std::clamp(widthRatio, 1.0, heightRatio));

  return static_cast<i32>(scale);
}

static auto integer_scale_centered_rect(Vec2 availableSize, Vec2 textureSize) -> SDL_FRect {
  auto scale = integer_scale(availableSize, textureSize);

  auto w = static_cast<float>(textureSize.height * scale);
  auto h = static_cast<float>(textureSize.width * scale);

  float x = (availableSize.width / 2) - (w / 2);
  float y = (availableSize.height / 2) - (h / 2);

  return {.x = x, .y = y, .w = w, .h = h};
}
