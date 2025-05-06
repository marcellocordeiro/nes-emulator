#pragma once

#include <memory>

#include "sdl_error.hpp"
#include "sdl_window.hpp"

namespace SDL {
class Renderer {
public:
  [[nodiscard]]
  Renderer() = default;

  [[nodiscard]]
  explicit Renderer(const Window& window) {
    auto* raw = SDL_CreateRenderer(window.get(), nullptr);

    if (raw == nullptr) {
      throw Error::fromContextWithSource("SDL_CreateRenderer");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Renderer* {
    return pointer.get();
  }

  void enableVsync() const {
    auto result = SDL_SetRenderVSync(pointer.get(), 1);

    if (!result) {
      throw Error::fromContextWithSource("SDL_SetRenderVSync");
    }
  }

private:
  struct Deleter {
    void operator()(SDL_Renderer* ptr) {
      if (ptr) {
        SDL_DestroyRenderer(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Renderer, Deleter>;

  Pointer pointer;
};
} // namespace SDL
