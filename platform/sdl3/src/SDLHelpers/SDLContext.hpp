#pragma once

#include <SDL3/SDL.h>

#include "SDLError.hpp"

namespace SDLHelpers {
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

private:
  static inline int counter = 0;
};
}
