#pragma once

#include <memory>

#include "lib/common.hpp"
#include "sdl_error.hpp"
#include "sdl_include.hpp"

namespace SDL {
class Window {
public:
  [[nodiscard]]
  Window(const std::string& title, i32 width, i32 height, SDL_WindowFlags flags) {
    auto* raw = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (raw == nullptr) {
      throw Error::from_context_with_source("SDL_CreateWindow");
    }

    pointer.reset(raw);
  }

  [[nodiscard]]
  auto get() const -> SDL_Window* {
    return pointer.get();
  }

  void set_window_position(const i32 x, const i32 y) const {
    const auto result = SDL_SetWindowPosition(pointer.get(), x, y);

    if (!result) {
      throw Error::from_context_with_source("SDL_SetWindowPosition");
    }
  }

  void show_window() const {
    const auto result = SDL_ShowWindow(pointer.get());

    if (!result) {
      throw Error::from_context_with_source("SDL_ShowWindow");
    }
  }

private:
  struct Deleter {
    void operator()(SDL_Window* ptr) const {
      if (ptr != nullptr) {
        SDL_DestroyWindow(ptr);
      }
    }
  };

  using Pointer = std::unique_ptr<SDL_Window, Deleter>;

  Pointer pointer;
};
} // namespace SDL
