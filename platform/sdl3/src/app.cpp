#include "app.hpp"

#include <chrono>
#include <format>

#include <nes/nes.h>

App::App(std::span<std::string_view> args) : args(args) {
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

auto App::run() -> void {
  SDL_WindowFlags window_flags = 0;

  window = SDL::Window(std::string(Nes::title), Nes::width * 3, Nes::height * 3, window_flags);
  renderer = SDL::Renderer(window);
  renderer.enableVsync();

  //SDL_SetRenderLogicalPresentation(renderer.get(), Nes::width, Nes::height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

  texture = SDL::Texture(
    renderer,
    SDL_PIXELFORMAT_XRGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    Nes::width,
    Nes::height
  );

  texture.setScaleMode(SDL_SCALEMODE_NEAREST);

  keys = SDL_GetKeyboardState(nullptr);

  setupDefaultBindings();

  Nes::set_app_path(SDL_GetBasePath());
  Nes::load(args[1]);
  Nes::power_on();

  auto fpsTimer = std::chrono::steady_clock::now();
  i32 elapsedFrames = 0;

  running = true;

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_EVENT_QUIT: Nes::power_off(); return;
      case SDL_EVENT_KEY_DOWN: processInput(e.key); break;

      default: break;
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

auto App::setupDefaultBindings() -> void {
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

auto App::updateEmulatedControllers() -> void {
  u8 state = 0;

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

auto App::processInput(const SDL_KeyboardEvent& key_event) -> void {
  auto key = key_event.scancode;

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

auto App::render() -> void {
  SDL_UpdateTexture(texture.get(), nullptr, Nes::get_frame_buffer(), Nes::width * sizeof(u32));

  SDL_RenderClear(renderer.get());
  SDL_RenderTexture(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
}
