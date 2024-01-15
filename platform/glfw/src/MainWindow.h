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
  MainWindow(int, char*[]);
  ~MainWindow();

  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&) = delete;
  auto operator=(const MainWindow&) -> MainWindow& = delete;
  auto operator=(MainWindow&&) -> MainWindow& = delete;

  void show();

  void run();
  void processInput();

private:
  std::vector<std::string_view> args;

  GLFWwindow* window = nullptr;
  Renderer renderer;

  std::array<std::uint8_t, 2> controllerState = {};
};
