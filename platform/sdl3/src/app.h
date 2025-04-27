#pragma once

#include <map>
#include <span>
#include <string_view>

#include "sdl/sdl.hpp"

class App {
public:
  explicit App(std::span<std::string_view> args);
  ~App() = default;

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
  auto processInput(const SDL_KeyboardEvent& key_event) -> void;

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
};
