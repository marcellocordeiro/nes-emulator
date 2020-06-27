#pragma once

#include <cstdint>

#include <glad/glad.h>

#include "Shader.h"

class Renderer {
public:
  Renderer() = default;
  ~Renderer();

  void render();

  void init();
  void set_size(int new_width, int new_height);
  void set_buffer(const uint32_t* ptr);

private:
  int width  = 0;
  int height = 0;

  Shader shader;

  GLuint texture = 0;
  GLuint VBO     = 0;
  GLuint VAO     = 0;
  GLuint EBO     = 0;

  const uint32_t* buffer = nullptr;
};
