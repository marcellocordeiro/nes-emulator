#include "MainWindow.h"

#include <format>
#include <numeric>

#include <nes/nes.h>

MainWindow::MainWindow(int argc, char* argv[]) : args(argv, argv + argc) {
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

auto MainWindow::show() -> int {
  {
    auto raw_window = SDL_CreateWindow(
      Nes::title.data(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      Nes::width * 3,
      Nes::height * 3,
      0
    );

    if (raw_window == nullptr) {
      printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
      return -1;
    }

    window.reset(raw_window);
  }

  {
    auto raw_renderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (raw_renderer == nullptr) {
      printf("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
      return -1;
    }

    renderer.reset(raw_renderer);
  }

  SDL_RenderSetLogicalSize(renderer.get(), Nes::width, Nes::height);

  {
    auto raw_texture =
      SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Nes::width, Nes::height);

    if (raw_texture == nullptr) {
      printf("Error: SDL_CreateTexture(): %s\n", SDL_GetError());
      return -1;
    }

    texture.reset(raw_texture);
  }

  keys = SDL_GetKeyboardState(nullptr);

  setupDefaultBindings();

  return 0;
}

auto MainWindow::run() -> void {
  buffer = Nes::get_back_buffer();

  Nes::set_app_path(SDL_GetBasePath());
  Nes::load(args[1]);
  Nes::power_on();

  auto fpsTimer = std::chrono::steady_clock::now();
  int elapsedFrames = 0;

  running = true;

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: Nes::power_off(); return;
      case SDL_KEYDOWN: processInput(e.key); break;
      }
    }

    {
      using namespace std::chrono_literals;

      auto elapsedTime = std::chrono::steady_clock::now() - fpsTimer;

      if (elapsedTime > 1s) {
        auto fps = elapsedFrames / std::chrono::duration<double>(elapsedTime).count();
        auto title = std::format("{} | {:5.2f}fps", Nes::title, fps);
        SDL_SetWindowTitle(window.get(), title.c_str());

        fpsTimer = std::chrono::steady_clock::now();
        elapsedFrames = 0;
      }
    }

    if (running) {
      updateEmulatedControllers();
      Nes::run_frame();
    }

    render();
    ++elapsedFrames;
  }
}

auto MainWindow::setupDefaultBindings() -> void {
  actionKeyBindings[Action::Pause] = SDL_SCANCODE_ESCAPE;
  actionKeyBindings[Action::Reset] = SDL_SCANCODE_R;
  actionKeyBindings[Action::SaveSnapshot] = SDL_SCANCODE_F1;
  actionKeyBindings[Action::LoadSnapshot] = SDL_SCANCODE_F3;
  actionKeyBindings[Action::ToggleLimiter] = SDL_SCANCODE_TAB;
  actionKeyBindings[Action::VolumeUp] = SDL_SCANCODE_KP_PLUS;
  actionKeyBindings[Action::VolumeDown] = SDL_SCANCODE_KP_MINUS;

  controllerKeyBindings[Button::A] = SDL_SCANCODE_A;
  controllerKeyBindings[Button::B] = SDL_SCANCODE_S;
  controllerKeyBindings[Button::Select] = SDL_SCANCODE_BACKSPACE;
  controllerKeyBindings[Button::Start] = SDL_SCANCODE_RETURN;
  controllerKeyBindings[Button::Up] = SDL_SCANCODE_UP;
  controllerKeyBindings[Button::Down] = SDL_SCANCODE_DOWN;
  controllerKeyBindings[Button::Left] = SDL_SCANCODE_LEFT;
  controllerKeyBindings[Button::Right] = SDL_SCANCODE_RIGHT;
}

auto MainWindow::updateEmulatedControllers() -> void {
  uint8_t state = 0;

  state |= (keys[controllerKeyBindings[Button::A]]) << 0;
  state |= (keys[controllerKeyBindings[Button::B]]) << 1;
  state |= (keys[controllerKeyBindings[Button::Select]]) << 2;
  state |= (keys[controllerKeyBindings[Button::Start]]) << 3;
  state |= (keys[controllerKeyBindings[Button::Up]]) << 4;
  state |= (keys[controllerKeyBindings[Button::Down]]) << 5;
  state |= (keys[controllerKeyBindings[Button::Left]]) << 6;
  state |= (keys[controllerKeyBindings[Button::Right]]) << 7;

  Nes::update_controller_state(0, state);
}

auto MainWindow::processInput(SDL_KeyboardEvent& key_event) -> void {
  auto key = key_event.keysym.scancode;

  for (const auto& [action, mappedKey] : actionKeyBindings) {
    if (mappedKey != key) {
      continue;
    }

    switch (action) {
    case Action::Pause: running = !running; return;
    case Action::Reset: Nes::reset(); return;
    case Action::SaveSnapshot: Nes::save_snapshot(); return;
    case Action::LoadSnapshot: Nes::load_snapshot(); return;
    case Action::ToggleLimiter: return;
    case Action::VolumeUp: return;
    case Action::VolumeDown: return;
    }
  }
}

auto MainWindow::render() -> void {
  SDL_UpdateTexture(texture.get(), nullptr, buffer, Nes::width * sizeof(uint32_t));

  SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
}
