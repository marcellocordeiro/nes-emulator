#include "app.hpp"

#include <chrono>
#include <format>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

#include "lib/common.hpp"
#include "nes/nes.hpp"
#include "sdl/sdl.hpp"
#include "utils/scaling.hpp"

using nes::Nes;

namespace {
void render_display(const SDL::Renderer& renderer, const SDL::Texture& texture) {
  const auto available_size = renderer.get_current_render_output_size();
  const auto rect =
    integer_scale_centered_rect(available_size, {.width = Nes::width, .height = Nes::height});

  UNUSED(rect); // fix

  SDL_RenderTexture(renderer.get(), texture.get(), nullptr, nullptr);
}
} // namespace

App::App(std::span<std::string_view> args) : args(args) {
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

auto App::run() -> void {
  Nes::set_app_path(SDL_GetBasePath());
  Nes::load(args[1]);
  Nes::power_on();

  auto context = SDL::Context{SDL_INIT_VIDEO | SDL_INIT_GAMEPAD};

  const auto window_flags = SDL_WindowFlags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN};

  const auto window =
    SDL::Window(std::string(Nes::title), Nes::width * 3, Nes::height * 3, window_flags);
  const auto renderer = SDL::Renderer(window);

  renderer.enable_vsync();
  window.set_window_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  window.show_window();

  const auto texture = SDL::Texture(
    renderer,
    SDL_PIXELFORMAT_XRGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    Nes::width,
    Nes::height
  );

  texture.set_scale_mode(SDL_SCALEMODE_NEAREST);

  keys = SDL_GetKeyboardState(nullptr);

  setupDefaultBindings();

  auto fpsTimer = std::chrono::steady_clock::now();
  i32 elapsedFrames = 0;

  running = true;

  while (true) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT: Nes::power_off(); return;
      case SDL_EVENT_KEY_DOWN: processInput(event.key); break;

      default: break;
      }

      if (SDL_GetWindowFlags(window.get()) & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(10);
        continue;
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

    SDL_UpdateTexture(texture.get(), nullptr, Nes::get_frame_buffer(), Nes::width * sizeof(u32));

    SDL_RenderClear(renderer.get());
    render_display(renderer, texture);
    SDL_RenderPresent(renderer.get());

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
