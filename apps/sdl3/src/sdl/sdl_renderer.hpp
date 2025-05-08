#pragma once

#include <memory>

#include "../utils/vec2.hpp"
#include "sdl_error.hpp"
#include "sdl_include.hpp"
#include "sdl_window.hpp"

namespace SDL {
class Renderer {
public:
  [[nodiscard]]
  explicit Renderer(const Window& window) {
    auto* raw = SDL_CreateRenderer(window.get(), nullptr);

    if (raw == nullptr) {
      throw Error::from_context_with_source("SDL_CreateRenderer");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Renderer* {
    return pointer.get();
  }

  void enable_vsync() const {
    const auto result = SDL_SetRenderVSync(pointer.get(), 1);

    if (!result) {
      throw Error::from_context_with_source("SDL_SetRenderVSync");
    }
  }

  [[nodiscard]]
  auto get_current_render_output_size() const -> Vec2 {
    int w = 0;
    int h = 0;

    const auto result = SDL_GetCurrentRenderOutputSize(pointer.get(), &w, &h);

    if (!result) {
      throw Error::from_context_with_source("SDL_GetCurrentRenderOutputSize");
    }

    return {.width = w, .height = h};
  }

private:
  struct Deleter {
    void operator()(SDL_Renderer* ptr) const {
      if (ptr != nullptr) {
        SDL_DestroyRenderer(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Renderer, Deleter>;

  Pointer pointer;
};
} // namespace SDL
