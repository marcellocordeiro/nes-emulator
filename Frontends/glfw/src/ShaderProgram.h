#pragma once

#include <glad/gl.h>

class ShaderProgram {
public:
  ShaderProgram() = default;
  ~ShaderProgram();

  ShaderProgram& init();
  ShaderProgram& add(const char*, GLenum);
  void           link();

  void use();

private:
  GLuint compile(const char*, GLenum);

  GLuint id = 0;
};
