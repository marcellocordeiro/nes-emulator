#pragma once

#include <memory>
#include <string_view>

#include <SDL3/SDL.h>

#include "SDLContext.hpp"
#include "SDLError.hpp"

namespace SDLHelpers {
class Window {
public:
  [[nodiscard]]
  Window() {}

  [[nodiscard]]
  Window(std::string title, int width, int height, SDL_WindowFlags flags) {
    auto raw = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (raw == nullptr) {
      throw Error::fromContextWithSource("SDL_CreateWindow");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Window* {
    return pointer.get();
  }

private:
  struct Deleter {
    void operator()(SDL_Window* ptr) {
      if (ptr) {
        SDL_DestroyWindow(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Window, Deleter>;

  Pointer pointer;
};

class Renderer {
public:
  [[nodiscard]]
  Renderer() {}

  [[nodiscard]]
  Renderer(const Window& window) {
    auto raw = SDL_CreateRenderer(window.get(), nullptr);

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

class Texture {
public:
  [[nodiscard]]
  Texture() {}

  [[nodiscard]]
  Texture(
    const Renderer& renderer,
    SDL_PixelFormat format,
    SDL_TextureAccess access,
    int width,
    int height
  ) {
    auto raw = SDL_CreateTexture(renderer.get(), format, access, width, height);

    if (raw == nullptr) {
      throw Error::fromContextWithSource("SDL_CreateTexture");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Texture* {
    return pointer.get();
  }

  void setScaleMode(SDL_ScaleMode mode) const {
    SDL_SetTextureScaleMode(pointer.get(), mode);
  }

private:
  struct Deleter {
    void operator()(SDL_Texture* ptr) {
      if (ptr) {
        SDL_DestroyTexture(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Texture, Deleter>;

  Pointer pointer;
};
} // namespace SDLHelpers
