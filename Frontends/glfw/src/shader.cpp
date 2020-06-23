#include "shader.h"

#include <fmt/format.h>

const char *vertex_shaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char *fragment_shaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, TexCoord);
}
)";

shader::~shader()
{
  if (!configured) return;

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
}

void shader::configure()
{
  auto check_compilation = [](GLuint s, std::string_view type) {
    GLint  success;
    GLchar infoLog[512];

    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(s, 512, nullptr, infoLog);
      fmt::print("{} shader compilation failed: {}\n", type, infoLog);
      throw;
    }
  };

  auto check_linking = [](int program) {
    GLint  success;
    GLchar infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program, 512, nullptr, infoLog);
      fmt::print("Shader program linking failed: {}\n", infoLog);
      throw;
    }
  };

  GLuint vertex_shader   = 0;
  GLuint fragment_shader = 0;

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shaderSource, nullptr);
  glCompileShader(vertex_shader);
  check_compilation(vertex_shader, "Vertex");

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shaderSource, nullptr);
  glCompileShader(fragment_shader);
  check_compilation(fragment_shader, "Fragment");

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex_shader);
  glAttachShader(shaderProgram, fragment_shader);
  glLinkProgram(shaderProgram);
  check_linking(shaderProgram);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Texture coordinates are switched (\/ +y, -> +x)
  float vertices[] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // Bottom left
      -1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  // Top left
      1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  // Top right
      1.0f,  -1.0f, 0.0f, 1.0f, 1.0f   // Bottom right
  };

  unsigned int indices[] = {
      0, 1, 3,  // First triangle
      1, 2, 3   // Second triangle
  };

  // Setup vertex data and buffers and configure
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
  // buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
  // it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  configured = true;
}

void shader::use()
{
  // draw our first triangle
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no need to bind it every time, but we'll do
                           // so to keep things a bit more organized
}
