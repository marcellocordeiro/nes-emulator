#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

#include "SDL2Helper.hpp"

class MainWindow {
public:
  MainWindow(int, char*[]);

  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&)      = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;

  auto show() -> void;
  auto run() -> void;

private:
  //
  // Settings
  //

  std::vector<std::string_view> args;

  SDL2Helper instance{SDL_INIT_VIDEO};

  SDL2Helper::Window window;

  // double volume = 0.1;
  bool running = false;

  //
  // Input
  //

  auto setupDefaultBindings() -> void;
  auto updateEmulatedControllers() -> void;
  auto processInput(SDL_KeyboardEvent&) -> void;

  enum class Button
  {
    A,
    B,
    Select,
    Start,
    Up,
    Down,
    Left,
    Right
  };
  enum class Action
  {
    Pause,
    Reset,
    SaveSnapshot,
    LoadSnapshot,
    ToggleLimiter,
    VolumeUp,
    VolumeDown
  };

  std::map<Action, SDL_Scancode> actionKeyBindings;
  std::map<Button, SDL_Scancode> controllerKeyBindings;

  const uint8_t* keys = nullptr;

  //
  // Renderer
  //

  auto render() -> void;

  SDL2Helper::Renderer renderer;
  SDL2Helper::Texture  texture;

  const uint32_t* buffer = nullptr;
};
