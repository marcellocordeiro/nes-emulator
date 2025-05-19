#pragma once

#include <map>
#include <span>
#include <string_view>

#include "nes/nes.hpp"
#include "sdl/sdl.hpp"

class App {
public:
  explicit App(std::span<std::string_view> args);

  void run();

private:
  //
  // Settings
  //

  std::string_view rom_path;

  // double volume = 0.1;
  bool running = false;

  //
  // Input
  //

  void setup_default_bindings();
  void update_emulated_controllers(nes::Nes& nes);
  void process_input(const SDL_KeyboardEvent& key_event, nes::Nes& nes);

  enum class Button {
    A,
    B,
    Select,
    Start,
    Up,
    Down,
    Left,
    Right,
  };

  enum class Action {
    Pause,
    Reset,
    SaveSnapshot,
    LoadSnapshot,
    ToggleLimiter,
    VolumeUp,
    VolumeDown,
  };

  std::map<Action, SDL_Scancode> action_key_bindings;
  std::map<Button, SDL_Scancode> controller_key_bindings;

  std::span<const bool> keys;
};
