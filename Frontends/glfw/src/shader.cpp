#include "Shader.h"

#include <fmt/format.h>

Shader::~Shader() { glDeleteProgram(shader_program); }

Shader& Shader::init()
{
  shader_program = glCreateProgram();
  return (*this);
}

Shader& Shader::attach(const char* source, GLenum type)
{
  auto shader = compile(source, type);
  glAttachShader(shader_program, shader);
  glDeleteShader(shader);

  return (*this);
}

GLuint Shader::compile(const char* source, GLenum type)
{
  auto check_compilation = [](GLuint s, GLenum type) {
    GLint  success;
    GLchar infoLog[512];

    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(s, 512, nullptr, infoLog);

      if (type == GL_VERTEX_SHADER) {
        fmt::print("Vertex shader compilation failed: {}\n", infoLog);
      } else if (type == GL_FRAGMENT_SHADER) {
        fmt::print("Fragment shader compilation failed: {}\n", infoLog);
      }

      throw;
    }
  };

  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  check_compilation(shader, type);

  return shader;
}

void Shader::link()
{
  auto check_linking = [](GLuint program) {
    GLint  success;
    GLchar infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program, 512, nullptr, infoLog);
      fmt::print("Shader program linking failed: {}\n", infoLog);
      throw;
    }
  };

  glLinkProgram(shader_program);
  check_linking(shader_program);
}

void Shader::use() { glUseProgram(shader_program); }
