#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <span>

#include "sdl/sdl.hpp"

class App {
public:
  App(std::span<std::string_view> args);

  App(const App&) = delete;
  App(App&&) = delete;
  App& operator=(const App&) = delete;
  App& operator=(App&&) = delete;

  auto run() -> void;

private:
  //
  // Settings
  //

  std::span<std::string_view> args;

  SDL::Context context{SDL_INIT_VIDEO};

  SDL::Window window;

  // double volume = 0.1;
  bool running = false;

  //
  // Input
  //

  auto setupDefaultBindings() -> void;
  auto updateEmulatedControllers() -> void;
  auto processInput(SDL_KeyboardEvent&) -> void;

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

  std::map<Action, SDL_Scancode> actionKeyBindings;
  std::map<Button, SDL_Scancode> controllerKeyBindings;

  const bool* keys = nullptr;

  //
  // Renderer
  //

  auto render() -> void;

  SDL::Renderer renderer;
  SDL::Texture texture;

  const uint32_t* buffer = nullptr;
};
