#include "MainWindow.h"

#include <fmt/core.h>
#include <nes/Emulator.h>

using namespace std::chrono_literals;

MainWindow::MainWindow(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

MainWindow::~MainWindow()
{
  if (texture) SDL_DestroyTexture(texture);
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);

  SDL_Quit();
}

void MainWindow::show()
{
  SDL_Init(SDL_INIT_VIDEO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window = SDL_CreateWindow(Emulator::title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Emulator::width * 3,
                            Emulator::height * 3, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetLogicalSize(renderer, Emulator::width, Emulator::height);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Emulator::width,
                              Emulator::height);

  keys = SDL_GetKeyboardState(nullptr);
}

void MainWindow::run()
{
  buffer = Emulator::get_back_buffer();

  Emulator::set_app_path(SDL_GetBasePath());
  Emulator::load(args[1]);
  Emulator::power_on();

  running   = true;
  fps_timer = std::chrono::steady_clock::now();

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: Emulator::power_off(); return;
        case SDL_KEYDOWN: process_keypress(e.key); break;
      }
    }

    if (!running) continue;

    auto elapsedTime = std::chrono::steady_clock::now() - fps_timer;

    if (elapsedTime > 1s) {
      auto fps   = elapsed_frames / std::chrono::duration<double>(elapsedTime).count();
      auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
      SDL_SetWindowTitle(window, title.c_str());

      fps_timer      = std::chrono::steady_clock::now();
      elapsed_frames = 0;
    }

    update_controllers();
    Emulator::run_frame();

    render();
    ++elapsed_frames;
  }
}

void MainWindow::process_keypress(SDL_KeyboardEvent& key_event)
{
  auto key = key_event.keysym.scancode;

  if (key == PAUSE) {
    running = !running;

    if (running) {
      auto title = fmt::format("{}", Emulator::title);
      SDL_SetWindowTitle(window, title.c_str());

      fps_timer      = std::chrono::steady_clock::now();
      elapsed_frames = 0;
    } else {
      auto title = fmt::format("{} | Paused", Emulator::title);
      SDL_SetWindowTitle(window, title.c_str());
    }
  } else if (key == RESET) {
    Emulator::reset();
  } else if (key == TOGGLE_LIMITER) {
    fps_limiter = !fps_limiter;
  } else if (key == SAVE_SNAPSHOT) {
    Emulator::save_snapshot();
  } else if (key == LOAD_SNAPSHOT) {
    Emulator::load_snapshot();
  } else if (key == VOLUME_UP) {
    volume = std::min(1.0, volume + 0.1);
    // Emulator::volume(volume);
  } else if (key == VOLUME_DOWN) {
    volume = std::max(0.0, volume - 0.1);
    // Emulator::volume(volume);
  }
}

void MainWindow::update_controllers()
{
  controller_state = 0;
  controller_state |= (keys[KEY_A]) << 0;
  controller_state |= (keys[KEY_B]) << 1;
  controller_state |= (keys[KEY_SELECT]) << 2;
  controller_state |= (keys[KEY_START]) << 3;
  controller_state |= (keys[KEY_UP]) << 4;
  controller_state |= (keys[KEY_DOWN]) << 5;
  controller_state |= (keys[KEY_LEFT]) << 6;
  controller_state |= (keys[KEY_RIGHT]) << 7;

  Emulator::update_controller_state(0, controller_state);
}

void MainWindow::render()
{
  SDL_UpdateTexture(texture, nullptr, buffer, Emulator::width * sizeof(std::uint32_t));

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
