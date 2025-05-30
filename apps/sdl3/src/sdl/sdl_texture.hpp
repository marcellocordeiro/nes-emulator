#pragma once

#include <memory>

#include "lib/common.hpp"
#include "sdl_error.hpp"
#include "sdl_include.hpp"
#include "sdl_renderer.hpp"

namespace sdl {
class Texture {
public:
  [[nodiscard]]
  Texture(
    const Renderer& renderer,
    const SDL_PixelFormat format,
    const SDL_TextureAccess access,
    const i32 width,
    const i32 height
  ) {
    auto* raw = SDL_CreateTexture(renderer.get(), format, access, width, height);

    if (raw == nullptr) {
      throw Error::from_context_with_source("SDL_CreateTexture");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Texture* {
    return pointer.get();
  }

  void set_scale_mode(const SDL_ScaleMode mode) const {
    const auto result = SDL_SetTextureScaleMode(pointer.get(), mode);

    if (!result) {
      throw Error::from_context_with_source("SDL_SetTextureScaleMode");
    }
  }

private:
  struct Deleter {
    void operator()(SDL_Texture* ptr) const {
      if (ptr != nullptr) {
        SDL_DestroyTexture(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Texture, Deleter>;

  Pointer pointer;
};
} // namespace sdl
