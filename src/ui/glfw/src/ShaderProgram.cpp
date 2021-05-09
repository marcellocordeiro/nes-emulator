#include "ShaderProgram.h"

#include <fmt/format.h>

ShaderProgram::~ShaderProgram() { glDeleteProgram(id); }

auto ShaderProgram::init() -> ShaderProgram&
{
  id = glCreateProgram();
  return (*this);
}

auto ShaderProgram::add(const char* source, GLenum type) -> ShaderProgram&
{
  auto shader = compile(source, type);
  glAttachShader(id, shader);
  glDeleteShader(shader);

  return (*this);
}

auto ShaderProgram::compile(const char* source, GLenum type) -> GLuint
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

void ShaderProgram::link()
{
  glLinkProgram(id);

  {  // Error checking
    GLint  success;
    GLchar infoLog[512];

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id, 512, nullptr, infoLog);
      fmt::print("Shader program linking failed: {}\n", infoLog);
      throw;
    }
  }
}

void ShaderProgram::use() { glUseProgram(id); }
