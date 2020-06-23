#pragma once

#include <GL/glew.h>

class shader {
public:
  shader() = default;
  ~shader();

  void configure();
  void use();

private:
  bool configured = false;

  GLuint VBO = 0, VAO = 0, EBO = 0;
  GLuint shaderProgram = 0;
};
