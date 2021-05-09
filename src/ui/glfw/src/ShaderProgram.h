#pragma once

#include <glad/gl.h>

class ShaderProgram {
public:
  ShaderProgram() = default;
  ~ShaderProgram();

  auto init() -> ShaderProgram&;
  auto add(const char*, GLenum) -> ShaderProgram&;
  void link();

  void use();

private:
private:
  auto compile(const char*, GLenum) -> GLuint;

  GLuint id = 0;
};
