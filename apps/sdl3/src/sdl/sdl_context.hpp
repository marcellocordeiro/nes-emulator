#pragma once

#include "lib/common.hpp"
#include "sdl_error.hpp"
#include "sdl_include.hpp"

namespace SDL {
class Context {
public:
  explicit Context(SDL_InitFlags flags) {
    auto result = SDL_Init(flags);

    if (!result) {
      throw Error::from_context_with_source("SDL_Init");
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
  Context(Context&&) = delete;
  auto operator=(const Context&) -> Context& = delete;
  auto operator=(Context&&) -> Context& = delete;

private:
  static inline i32 counter = 0;
};
} // namespace SDL
