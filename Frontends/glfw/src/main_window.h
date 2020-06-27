#pragma once

#include <string_view>
#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

class main_window {
public:
  main_window(int, char *[]);
  ~main_window();

  void run();
  void processInput(GLFWwindow *window);

private:
  std::vector<std::string_view> args;

  std::chrono::time_point<std::chrono::steady_clock> fps_timer;
  int  elapsed_frames = 0;

  Renderer renderer;
  GLFWwindow *window = nullptr;
};
