#pragma once

#include "sdl_error.hpp"
#include "sdl_include.hpp"

namespace SDL {
class Context {
public:
  explicit Context(SDL_InitFlags flags) {
    auto result = SDL_Init(flags);

    if (!result) {
      throw Error::fromContextWithSource("SDL_Init");
    }

    ++counter;
  }

  ~Context() {
    --counter;

    if (!counter) {
      SDL_Quit();
    }
  }

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

private:
  static inline int counter = 0;
};
} // namespace SDL
