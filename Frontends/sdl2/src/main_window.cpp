#include "main_window.h"

#include <fmt/format.h>
#include <nes/Emulator.h>

#include "system_utils.h"

using namespace std::chrono_literals;

main_window::main_window(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  Emulator::set_app_path(lib::get_app_path());
  Emulator::load(args[1]);
  Emulator::power_on();
}

main_window::~main_window()
{
  SDL_PauseAudio(true);
  SDL_CloseAudio();

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}

void main_window::init()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window = SDL_CreateWindow("nes-emulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Emulator::width * 3,
                            Emulator::height * 3, 0);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetLogicalSize(renderer, Emulator::width, Emulator::height);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, Emulator::width,
                              Emulator::height);

  keys = SDL_GetKeyboardState(nullptr);

  /*sound_queue = std::make_unique<Sound_Queue>();
  sound_queue->init();

  SDL_AudioSpec want;
  SDL_zero(want);
  want.freq     = 44100;
  want.format   = AUDIO_S16SYS;
  want.channels = 1;
  want.samples  = Sound_Queue::buf_size;
  want.userdata = sound_queue.get();
  want.callback = [](void* user_data, Uint8* out, int count) {
    static_cast<Sound_Queue*>(user_data)->fill_buffer(out, count);
  };

  SDL_OpenAudio(&want, nullptr);
  SDL_PauseAudio(false);*/
}

void main_window::get_controllers()
{
  for (size_t i = 0; i < 2; ++i) {
    controller_state[i] = 0;
    controller_state[i] |= (keys[KEY_A[i]]) << 0;
    controller_state[i] |= (keys[KEY_B[i]]) << 1;
    controller_state[i] |= (keys[KEY_SELECT[i]]) << 2;
    controller_state[i] |= (keys[KEY_START[i]]) << 3;
    controller_state[i] |= (keys[KEY_UP[i]]) << 4;
    controller_state[i] |= (keys[KEY_DOWN[i]]) << 5;
    controller_state[i] |= (keys[KEY_LEFT[i]]) << 6;
    controller_state[i] |= (keys[KEY_RIGHT[i]]) << 7;
  }
}

void main_window::update_controllers()
{
  Emulator::update_controller_state(0, controller_state[0]);
}

void main_window::update_frame()
{
  SDL_UpdateTexture(texture, nullptr, Emulator::get_back_buffer(),
                    Emulator::width * sizeof(std::uint32_t));
}

void main_window::render()
{
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void main_window::get_samples()
{
  /*if (APU::get().samples_available(audio_buffer.size())) {
    auto sample_count =
        APU::get().get_samples(audio_buffer.data(), audio_buffer.size());
    sound_queue->write(audio_buffer.data(), sample_count);
  }*/
}

void main_window::process_keypress(SDL_KeyboardEvent& key_event)
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

void main_window::run()
{
  running   = true;
  fps_timer = std::chrono::steady_clock::now();
  // APU::get().volume(volume);

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
      auto fps =
          elapsed_frames / std::chrono::duration<double>(elapsedTime).count();
      auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
      SDL_SetWindowTitle(window, title.c_str());

      fps_timer      = std::chrono::steady_clock::now();
      elapsed_frames = 0;
    }

    get_controllers();
    update_controllers();
    Emulator::run_frame();

    get_samples();
    update_frame();

    ++elapsed_frames;

    render();
  }
}
