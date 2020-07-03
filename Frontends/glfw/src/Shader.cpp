#include "Shader.h"

#include <fmt/format.h>

Shader::~Shader() { glDeleteProgram(shaderProgram); }

Shader& Shader::init()
{
  shaderProgram = glCreateProgram();
  return (*this);
}

Shader& Shader::attach(const char* source, GLenum type)
{
  auto shader = compile(source, type);
  glAttachShader(shaderProgram, shader);
  glDeleteShader(shader);

  return (*this);
}

GLuint Shader::compile(const char* source, GLenum type)
{
  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  {  // Error checking
    GLint  success;
    GLchar infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);

      if (type == GL_VERTEX_SHADER) {
        fmt::print("Vertex shader compilation failed: {}\n", infoLog);
      } else if (type == GL_FRAGMENT_SHADER) {
        fmt::print("Fragment shader compilation failed: {}\n", infoLog);
      }

      throw;
    }
  }

  return shader;
}

void Shader::link()
{
  glLinkProgram(shaderProgram);

  {  // Error checking
    GLint  success;
    GLchar infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
      fmt::print("Shader program linking failed: {}\n", infoLog);
      throw;
    }
  }
}

void Shader::use() { glUseProgram(shaderProgram); }
