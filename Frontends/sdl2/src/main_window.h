#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

#define SDL_MAIN_HANDLED

#include <SDL.h>

class main_window {
public:
  main_window(int, char*[]);
  ~main_window();

  main_window(const main_window&) = delete;
  main_window(main_window&&)      = delete;
  main_window& operator=(const main_window&) = delete;
  main_window& operator=(main_window&&) = delete;

  void init();
  void run();

private:
  void render();
  void get_controllers();
  void update_controllers();
  void update_frame();

  void process_keypress(SDL_KeyboardEvent&);

  std::vector<std::string_view> args;

  double volume = 0.1;

  bool running        = false;
  bool fps_limiter    = true;
  int  elapsed_frames = 0;

  std::chrono::time_point<std::chrono::steady_clock> fps_timer;

  std::array<std::uint8_t, 2> controller_state = {};

  SDL_Window*         window   = nullptr;
  SDL_Renderer*       renderer = nullptr;
  SDL_Texture*        texture  = nullptr;
  const std::uint8_t* keys     = nullptr;

  const uint32_t* buffer = nullptr;

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
};
