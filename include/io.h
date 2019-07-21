#pragma once

#include <memory>

#include <SDL.h>

#include "types/forward_decl.h"
#include "common.h"

namespace SDL2 {
struct Deleter {
  void operator()(SDL_Window* ptr)
  {
    if (ptr) {
      SDL_DestroyWindow(ptr);
    }
  }

  void operator()(SDL_Renderer* ptr)
  {
    if (ptr) {
      SDL_DestroyRenderer(ptr);
    }
  }

  void operator()(SDL_Texture* ptr)
  {
    if (ptr) {
      SDL_DestroyTexture(ptr);
    }
  }
};

using Window   = std::unique_ptr<SDL_Window, Deleter>;
using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
using Texture  = std::unique_ptr<SDL_Texture, Deleter>;
}  // namespace SDL2

namespace nes {
class io {
public:
  io(nes::cpu&, nes::cartridge&);
  ~io();

  void close();

  uint8_t get_controller(size_t) const;
  void    update_frame(const uint32_t*);
  void    draw();

  void run();

private:
  nes::cpu& cpu;
  nes::cartridge& cartridge;

  static constexpr int width  = 256;
  static constexpr int height = 240;

  SDL2::Window   window;
  SDL2::Renderer renderer;
  SDL2::Texture  texture;
  const uint8_t* keys;

  SDL_Scancode KEY_A[2]      = {SDL_SCANCODE_A, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_B[2]      = {SDL_SCANCODE_S, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_SELECT[2] = {SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_START[2]  = {SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_UP[2]     = {SDL_SCANCODE_UP, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_DOWN[2]   = {SDL_SCANCODE_DOWN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_LEFT[2]   = {SDL_SCANCODE_LEFT, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_RIGHT[2]  = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_ESCAPE};
};
}  // namespace nes
