#pragma once

#include <glad/glad.h>

class Shader {
public:
  Shader() = default;
  ~Shader();

  Shader& init();
  Shader& attach(const char*, GLenum);
  void    link();

  void use();

private:
  GLuint compile(const char*, GLenum);

  GLuint shader_program = 0;
};
