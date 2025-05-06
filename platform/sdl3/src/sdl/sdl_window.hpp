#pragma once

#include <memory>

#include "lib/common.hpp"
#include "sdl_error.hpp"

namespace SDL {
class Window {
public:
  [[nodiscard]]
  Window() = default;

  [[nodiscard]]
  Window(const std::string& title, i32 width, i32 height, SDL_WindowFlags flags) {
    auto* raw = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (raw == nullptr) {
      throw Error::fromContextWithSource("SDL_CreateWindow");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Window* {
    return pointer.get();
  }

  void setWindowPosition(i32 x, i32 y) {
    auto result = SDL_SetWindowPosition(pointer.get(), x, y);

    if (!result) {
      throw Error::fromContextWithSource("SDL_SetWindowPosition");
    }
  }

  void showWindow() {
    auto result = SDL_ShowWindow(pointer.get());

    if (!result) {
      throw Error::fromContextWithSource("SDL_ShowWindow");
    }
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
} // namespace SDL
