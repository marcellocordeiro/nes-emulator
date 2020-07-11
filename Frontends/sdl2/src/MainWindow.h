#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL.h>

class MainWindow {
public:
  MainWindow(int, char*[]);
  ~MainWindow();

  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&)      = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;

  void show();
  void run();

private:
  void process_keypress(SDL_KeyboardEvent&);
  void update_controllers();
  void render();

  std::vector<std::string_view> args;

  double volume = 0.1;

  bool running        = false;
  bool fps_limiter    = true;
  int  elapsed_frames = 0;

  std::chrono::time_point<std::chrono::steady_clock> fps_timer;

  std::uint8_t controller_state = 0;

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

  SDL_Scancode KEY_A      = SDL_SCANCODE_A;
  SDL_Scancode KEY_B      = SDL_SCANCODE_S;
  SDL_Scancode KEY_SELECT = SDL_SCANCODE_BACKSPACE;
  SDL_Scancode KEY_START  = SDL_SCANCODE_RETURN;
  SDL_Scancode KEY_UP     = SDL_SCANCODE_UP;
  SDL_Scancode KEY_DOWN   = SDL_SCANCODE_DOWN;
  SDL_Scancode KEY_LEFT   = SDL_SCANCODE_LEFT;
  SDL_Scancode KEY_RIGHT  = SDL_SCANCODE_RIGHT;
};
