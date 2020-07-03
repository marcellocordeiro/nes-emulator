#include "Renderer.h"

const char *vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char *fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, TexCoord);
}
)";

Renderer::~Renderer()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Renderer::init()
{
  shader.init().attach(vertexShaderSource, GL_VERTEX_SHADER).attach(fragmentShaderSource, GL_FRAGMENT_SHADER).link();

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

  glBindVertexArray(VAO);

  // Configure texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Renderer::render()
{
  // glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

  shader.use();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::setBuffer(const uint32_t *ptr, int width, int height)
{
  buffer       = ptr;
  bufferWidth  = width;
  bufferHeight = height;
}

void Renderer::resize(int width, int height) { glViewport(0, 0, width, height); }
