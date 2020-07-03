#pragma once

#include <array>
#include <chrono>
#include <string_view>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Renderer.h"

class MainWindow {
public:
  MainWindow() = default;
  ~MainWindow();

  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&)      = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;

  void setArgs(int, char*[]);
  void show();

  void run();
  void processInput();

private:
  std::vector<std::string_view> args;

  std::chrono::time_point<std::chrono::steady_clock> fpsTimer;

  int elapsedFrames = 0;

  GLFWwindow* window = nullptr;
  Renderer    renderer;

  std::array<std::uint8_t, 2> controllerState = {};
};
