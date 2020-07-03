#pragma once

#include <cstdint>

#include <glad/glad.h>

#include "ShaderProgram.h"

class Renderer {
public:
  Renderer() = default;
  ~Renderer();

  void render();

  void init();
  void setBuffer(const uint32_t*, int, int);
  void resize(int, int);

private:
  int bufferWidth  = 0;
  int bufferHeight = 0;

  ShaderProgram shaderProgram;

  GLuint texture = 0;
  GLuint VBO     = 0;
  GLuint VAO     = 0;
  GLuint EBO     = 0;

  const uint32_t* buffer = nullptr;
};
