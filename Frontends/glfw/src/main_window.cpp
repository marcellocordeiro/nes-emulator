#include "main_window.h"

#include <fmt/format.h>

#include <nes/Emulator.h>
#include <system_utils.h>

main_window::main_window(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Emulator::width * 3, Emulator::height * 3, Emulator::title.data(), nullptr, nullptr);
  if (!window) {
    fmt::format("Failed to create GLFW window\n");
    glfwTerminate();
    throw;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
  });

  if (glewInit() != GLEW_OK) {
    fmt::format("Failed to initialize GLEW\n");
    throw;
  }

  Emulator::set_app_path(lib::get_base_path());
  Emulator::load(args[1]);
  Emulator::power_on();

  buffer = Emulator::get_back_buffer();

  my_shader.configure();

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

main_window::~main_window() { glfwTerminate(); }

void main_window::run()
{
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    Emulator::run_frame();

    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Emulator::width, Emulator::height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                 buffer);

    my_shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }
}
