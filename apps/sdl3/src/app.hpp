#pragma once

#include <map>
#include <span>
#include <string_view>

#include "sdl/sdl.hpp"

class App {
public:
  explicit App(std::span<std::string_view> args);

  void run();

private:
  //
  // Settings
  //

  std::span<std::string_view> args;

  // double volume = 0.1;
  bool running = false;

  //
  // Input
  //

  void setup_default_bindings();

  void update_emulated_controllers();

  void process_input(const SDL_KeyboardEvent& key_event);

  enum class Button : u8 {
    A,
    B,
    Select,
    Start,
    Up,
    Down,
    Left,
    Right,
  };

  enum class Action : u8 {
    Pause,
    Reset,
    SaveSnapshot,
    LoadSnapshot,
    ToggleLimiter,
    VolumeUp,
    VolumeDown,
  };

  std::map<Action, SDL_Scancode> actionKeyBindings;
  std::map<Button, SDL_Scancode> controllerKeyBindings;

  const bool* keys = nullptr;
};
