#include "MainWindow.h"

#include <format>

#include <nes/Emulator.h>
#include <system_utils.h>

MainWindow::MainWindow(int argc, char* argv[]) {
  args = {argv, argv + argc};

  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

MainWindow::~MainWindow() { glfwTerminate(); }

void MainWindow::show() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(
    Emulator::width * 3,
    Emulator::height * 3,
    Emulator::title.data(),
    nullptr,
    nullptr
  );
  if (!window) {
    fmt::print("Failed to create GLFW window\n");
    throw;
  }
  glfwSetWindowUserPointer(window, this);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (!gladLoadGL(glfwGetProcAddress)) {
    fmt::print("Failed to initialize GLAD\n");
    throw;
  }

  glfwSetFramebufferSizeCallback(
    window,
    [](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); }
  );

  glfwSetErrorCallback([](int, const char* description) {
    fmt::print("Error: {}\n", description);
  });
}

void MainWindow::run() {
  Emulator::set_app_path(lib::get_base_path());
  Emulator::load(args[1]);
  Emulator::power_on();

  renderer.init();
  renderer.setBuffer(
    Emulator::get_back_buffer(), Emulator::width, Emulator::height
  );

  auto fpsTimer = std::chrono::steady_clock::now();
  int elapsedFrames = 0;
  // std::chrono::time_point<std::chrono::steady_clock> fpsTimer;

  while (!glfwWindowShouldClose(window)) {
    processInput();

    {
      using namespace std::chrono_literals;

      auto elapsedTime = std::chrono::steady_clock::now() - fpsTimer;

      if (elapsedTime > 1s) {
        auto fps =
          elapsedFrames / std::chrono::duration<double>(elapsedTime).count();
        auto title = std::format("{} | {:5.2f}fps", Emulator::title, fps);
        glfwSetWindowTitle(window, title.c_str());

        fpsTimer = std::chrono::steady_clock::now();
        elapsedFrames = 0;
      }
    }

    Emulator::update_controller_state(0, controllerState[0]);
    Emulator::run_frame();

    renderer.render();
    ++elapsedFrames;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void MainWindow::processInput() {
  for (size_t i = 0; i < 2; ++i) {
    controllerState[i] = 0;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_A) << 0;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_S) << 1;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_BACKSPACE) << 2;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_ENTER) << 3;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_UP) << 4;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_DOWN) << 5;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_LEFT) << 6;
    controllerState[i] |= glfwGetKey(window, GLFW_KEY_RIGHT) << 7;
  }
}
