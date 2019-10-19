#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

#include <SDL.h>

// TODO: reimplement this
#include "Sound_Queue.h"

namespace nes {
class sdl2_frontend {
public:
  sdl2_frontend(int, char*[]);
  ~sdl2_frontend();

  sdl2_frontend(const sdl2_frontend&) = delete;
  sdl2_frontend(sdl2_frontend&&)      = delete;
  sdl2_frontend& operator=(const sdl2_frontend&) = delete;
  sdl2_frontend& operator=(sdl2_frontend&&) = delete;

  void init();
  void run();

private:
  void render();
  void get_controllers();
  void update_controllers();
  void update_frame();
  void get_samples();

  void process_keypress(SDL_KeyboardEvent&);

  std::vector<std::string_view> args;

  double volume = 0.1;

  bool running        = false;
  bool fps_limiter    = true;
  int  elapsed_frames = 0;

  std::array<std::uint8_t, 2> controller_state = {};

  SDL_Window*         window   = nullptr;
  SDL_Renderer*       renderer = nullptr;
  SDL_Texture*        texture  = nullptr;
  const std::uint8_t* keys     = nullptr;

  // TODO: reimplement this
  std::array<std::int16_t, 4096> audio_buffer = {};
  std::unique_ptr<Sound_Queue>   sound_queue;
};
}  // namespace nes
