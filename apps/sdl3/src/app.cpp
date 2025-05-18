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
void render_display(const sdl::Renderer& renderer, const sdl::Texture& texture) {
  const auto available_size = renderer.get_current_render_output_size();
  const auto rect =
    integer_scale_centered_rect(available_size, {.width = Nes::width, .height = Nes::height});

  UNUSED(rect); // fix

  SDL_RenderTexture(renderer.get(), texture.get(), nullptr, nullptr);
}
} // namespace

App::App(const std::span<std::string_view> args) : args(args) {
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }
}

void App::run() {
  auto nes = Nes();

  nes.set_app_path(SDL_GetBasePath());
  nes.load(args[1]);
  nes.power_on();

  auto context = sdl::Context{SDL_INIT_VIDEO | SDL_INIT_GAMEPAD};

  constexpr auto window_flags = SDL_WindowFlags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN};

  const auto window =
    sdl::Window(std::string(Nes::title), Nes::width * 3, Nes::height * 3, window_flags);
  const auto renderer = sdl::Renderer(window);

  renderer.enable_vsync();
  window.set_window_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  window.show_window();

  const auto texture = sdl::Texture(
    renderer,
    SDL_PIXELFORMAT_XRGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    Nes::width,
    Nes::height
  );

  texture.set_scale_mode(SDL_SCALEMODE_NEAREST);

  {
    int numkeys = 0;
    const auto raw_keys = SDL_GetKeyboardState(&numkeys);
    keys = std::span(raw_keys, static_cast<usize>(numkeys));
  }

  setup_default_bindings();

  auto fps_timer = std::chrono::steady_clock::now();
  i32 elapsed_frames = 0;

  running = true;

  while (true) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT: nes.power_off(); return;
      case SDL_EVENT_KEY_DOWN: process_input(event.key, nes); break;

      default: break;
      }

      if ((SDL_GetWindowFlags(window.get()) & SDL_WINDOW_MINIMIZED) != 0) {
        SDL_Delay(10);
        continue;
      }
    }

    {
      using namespace std::chrono_literals;

      auto elapsed_time = std::chrono::steady_clock::now() - fps_timer;

      if (elapsed_time > 1s) {
        auto fps = elapsed_frames / std::chrono::duration<double>(elapsed_time).count();
        auto title = std::format("{} | {:5.2f}fps", Nes::title, fps);
        SDL_SetWindowTitle(window.get(), title.c_str());

        fps_timer = std::chrono::steady_clock::now();
        elapsed_frames = 0;
      }
    }

    if (running) {
      update_emulated_controllers(nes);
      nes.run_frame();
    }

    SDL_UpdateTexture(texture.get(), nullptr, nes.get_frame_buffer(), Nes::width * sizeof(u32));

    SDL_RenderClear(renderer.get());
    render_display(renderer, texture);
    SDL_RenderPresent(renderer.get());

    ++elapsed_frames;
  }
}

void App::setup_default_bindings() {
  action_key_bindings[Action::Pause] = SDL_SCANCODE_ESCAPE;
  action_key_bindings[Action::Reset] = SDL_SCANCODE_R;
  action_key_bindings[Action::SaveSnapshot] = SDL_SCANCODE_F1;
  action_key_bindings[Action::LoadSnapshot] = SDL_SCANCODE_F3;
  action_key_bindings[Action::ToggleLimiter] = SDL_SCANCODE_TAB;
  action_key_bindings[Action::VolumeUp] = SDL_SCANCODE_KP_PLUS;
  action_key_bindings[Action::VolumeDown] = SDL_SCANCODE_KP_MINUS;

  controller_key_bindings[Button::A] = SDL_SCANCODE_X;
  controller_key_bindings[Button::B] = SDL_SCANCODE_Z;
  controller_key_bindings[Button::Select] = SDL_SCANCODE_BACKSPACE;
  controller_key_bindings[Button::Start] = SDL_SCANCODE_RETURN;
  controller_key_bindings[Button::Up] = SDL_SCANCODE_UP;
  controller_key_bindings[Button::Down] = SDL_SCANCODE_DOWN;
  controller_key_bindings[Button::Left] = SDL_SCANCODE_LEFT;
  controller_key_bindings[Button::Right] = SDL_SCANCODE_RIGHT;
}

void App::update_emulated_controllers(Nes& nes) {
  u8 state = 0;

  state |= static_cast<int>(keys[controller_key_bindings[Button::A]]) << 0;
  state |= static_cast<int>(keys[controller_key_bindings[Button::B]]) << 1;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Select]]) << 2;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Start]]) << 3;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Up]]) << 4;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Down]]) << 5;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Left]]) << 6;
  state |= static_cast<int>(keys[controller_key_bindings[Button::Right]]) << 7;

  nes.update_controller_state(0, state);
}

void App::process_input(const SDL_KeyboardEvent& key_event, Nes& nes) {
  const auto key = key_event.scancode;

  for (const auto& [action, mappedKey] : action_key_bindings) {
    if (mappedKey != key) {
      continue;
    }

    switch (action) {
    case Action::Pause: running = !running; return;
    case Action::Reset: nes.reset(); return;

    case Action::SaveSnapshot:
    case Action::LoadSnapshot:
    case Action::ToggleLimiter:
    case Action::VolumeUp:
    case Action::VolumeDown:
    default: return;
    }
  }
}
