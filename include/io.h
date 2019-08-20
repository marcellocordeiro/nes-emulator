#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include <SDL.h>

#include "common.h"
#include "types/forward_decl.h"

namespace SDL2 {
struct Instance {
  Instance()
  {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  }

  ~Instance()
  {
    SDL_Quit();
  }
};

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
  io(nes::emulator&);
  ~io();

  void start();
  void close();

  uint8_t get_controller(size_t) const;
  void    update_frame(std::array<uint32_t, 256 * 240>&);
  void    write_samples(int16_t*, long);  // todo: reimplement this
  void    render();

  void poll_events();
  void handle_keys();

  void run();
  void run_cpu();

private:
  nes::emulator& emulator;

  static constexpr int width  = 256;
  static constexpr int height = 240;

  std::array<uint32_t, 256 * 240> front_buffer{};

  double volume = 0.1;

  std::mutex        event_lock;
  std::mutex        render_lock;
  std::atomic<bool> frame_ready    = false;
  std::atomic<bool> pending_exit   = false;
  std::atomic<bool> running        = false;
  std::atomic<bool> fps_limiter    = true;
  std::atomic<int>  elapsed_frames = 0;

  SDL2::Window   window;
  SDL2::Renderer renderer;
  SDL2::Texture  texture;
  const uint8_t* keys;

  SDL2::Instance SDL2_guard;

  SDL_Scancode PAUSE          = SDL_SCANCODE_ESCAPE;
  SDL_Scancode RESET          = SDL_SCANCODE_R;
  SDL_Scancode SAVE_SNAPSHOT  = SDL_SCANCODE_F1;
  SDL_Scancode LOAD_SNAPSHOT  = SDL_SCANCODE_F3;
  SDL_Scancode TOGGLE_LIMITER = SDL_SCANCODE_TAB;

  SDL_Scancode VOLUME_UP   = SDL_SCANCODE_KP_PLUS;
  SDL_Scancode VOLUME_DOWN = SDL_SCANCODE_KP_MINUS;

  SDL_Scancode KEY_A[2]      = {SDL_SCANCODE_A, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_B[2]      = {SDL_SCANCODE_S, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_SELECT[2] = {SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_START[2]  = {SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_UP[2]     = {SDL_SCANCODE_UP, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_DOWN[2]   = {SDL_SCANCODE_DOWN, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_LEFT[2]   = {SDL_SCANCODE_LEFT, SDL_SCANCODE_ESCAPE};
  SDL_Scancode KEY_RIGHT[2]  = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_ESCAPE};

  // todo: reimplement this
  std::unique_ptr<Sound_Queue> sound_queue;
};
}  // namespace nes
