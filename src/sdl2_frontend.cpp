#include "sdl2_frontend.h"

#include <nes/Emulator.h>

sdl2_frontend::sdl2_frontend()
{
  SDL_Init(SDL_INIT_VIDEO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window = SDL_CreateWindow("nes-emulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Emulator::width * 2,
                            Emulator::height * 2, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(renderer, Emulator::width, Emulator::height);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, Emulator::width,
                              Emulator::height);
}

sdl2_frontend::~sdl2_frontend()
{
  SDL_PauseAudio(true);
  SDL_CloseAudio();

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}

void sdl2_frontend::render()
{
  SDL_UpdateTexture(texture, nullptr, Emulator::get_back_buffer(),
                    Emulator::width * sizeof(std::uint32_t));
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
