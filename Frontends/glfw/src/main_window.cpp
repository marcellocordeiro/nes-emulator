#include "main_window.h"

#include <fmt/format.h>

#include <nes/Emulator.h>
#include <system_utils.h>

using namespace std::chrono_literals;

main_window::main_window(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Emulator::width * 3, Emulator::height * 3, Emulator::title.data(), nullptr, nullptr);
  if (!window) {
    fmt::format("Failed to create GLFW window\n");
    glfwTerminate();
    throw;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height);
  // });

  if (!gladLoadGL()) {
    fmt::print("Failed to initialize GLAD\n");
    throw;
  }

  Emulator::set_app_path(lib::get_base_path());
  Emulator::load(args[1]);
  Emulator::power_on();

  renderer.init();
  renderer.set_buffer(Emulator::get_back_buffer());
  renderer.set_size(Emulator::width, Emulator::height);
}

main_window::~main_window() { glfwTerminate(); }

void main_window::run()
{
  fps_timer = std::chrono::steady_clock::now();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    auto elapsedTime = std::chrono::steady_clock::now() - fps_timer;

    if (elapsedTime > 1s) {
      auto fps   = elapsed_frames / std::chrono::duration<double>(elapsedTime).count();
      auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
      glfwSetWindowTitle(window, title.c_str());

      fps_timer      = std::chrono::steady_clock::now();
      elapsed_frames = 0;
    }

    Emulator::run_frame();

    renderer.render();
    ++elapsed_frames;

    glfwSwapBuffers(window);
  }
}
