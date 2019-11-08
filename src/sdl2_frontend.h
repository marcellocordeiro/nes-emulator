#pragma once

#include <SDL.h>

class sdl2_frontend {
public:
  sdl2_frontend();
  ~sdl2_frontend();

  sdl2_frontend(const sdl2_frontend&) = delete;
  sdl2_frontend(sdl2_frontend&&)      = delete;
  sdl2_frontend& operator=(const sdl2_frontend&) = delete;
  sdl2_frontend& operator=(sdl2_frontend&&) = delete;

private:
  void render();

  SDL_Window*   window   = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture*  texture  = nullptr;
};
