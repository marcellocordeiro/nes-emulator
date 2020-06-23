#pragma once

#include <string_view>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class main_window {
public:
  main_window(int, char *[]);
  ~main_window();

  void run();
  void processInput(GLFWwindow *window);

private:
  std::vector<std::string_view> args;

  GLFWwindow *window = nullptr;
  shader      my_shader;

  GLuint texture = 0;

  const uint32_t *buffer = nullptr;
};
