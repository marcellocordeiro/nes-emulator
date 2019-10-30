#include "sdl2_frontend.h"

#include <fmt/format.h>

#include <nes/Emulator.h>
#include <timer.h>

using namespace std::chrono;
using namespace std::chrono_literals;

namespace nes {
sdl2_frontend::sdl2_frontend(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (args.size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  Utility::FileManager::get().setup();
  Utility::FileManager::get().set_rom(args[1]);
}

sdl2_frontend::~sdl2_frontend()
{
  SDL_PauseAudio(true);
  SDL_CloseAudio();

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}

void sdl2_frontend::init()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window = SDL_CreateWindow("nes-emulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Emulator::width * 2,
                            Emulator::height * 2, 0);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetLogicalSize(renderer, Emulator::width, Emulator::height);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, Emulator::width,
                              Emulator::height);

  keys = SDL_GetKeyboardState(nullptr);

  sound_queue = std::make_unique<Sound_Queue>();
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
  SDL_PauseAudio(false);
}

void sdl2_frontend::get_controllers()
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

void sdl2_frontend::update_controllers()
{
  Controller::get().update_state(0, controller_state[0]);
}

void sdl2_frontend::update_frame()
{
  SDL_UpdateTexture(texture, nullptr, PPU::get().get_back_buffer().data(),
                    Emulator::width * sizeof(std::uint32_t));
}

void sdl2_frontend::render()
{
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void sdl2_frontend::get_samples()
{
  if (APU::get().samples_available(audio_buffer.size())) {
    auto sample_count =
        APU::get().get_samples(audio_buffer.data(), audio_buffer.size());
    sound_queue->write(audio_buffer.data(), sample_count);
  }
}

void sdl2_frontend::process_keypress(SDL_KeyboardEvent& key_event)
{
  auto key = key_event.keysym.scancode;

  if (key == PAUSE) {
    running = !running;
  } else if (key == RESET) {
    Emulator::get().reset();
  } else if (key == TOGGLE_LIMITER) {
    fps_limiter = !fps_limiter;
  } else if (key == SAVE_SNAPSHOT) {
    Emulator::get().save_snapshot();
  } else if (key == LOAD_SNAPSHOT) {
    Emulator::get().load_snapshot();
  } else if (key == VOLUME_UP) {
    volume = std::min(1.0, volume + 0.1);
    Emulator::get().volume(volume);
  } else if (key == VOLUME_DOWN) {
    volume = std::max(0.0, volume - 0.1);
    Emulator::get().volume(volume);
  }
}

void sdl2_frontend::run()
{
  Emulator::get().power_on();

  running = true;
  APU::get().volume(volume);

  lib::timer fps_timer;

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: Cartridge::get().dump_prg_ram(); return;
        case SDL_KEYDOWN: process_keypress(e.key); break;
      }
    }

    if (fps_timer.elapsed_time() > 1s) {
      if (running) {
        auto fps =
            elapsed_frames / duration<double>(fps_timer.elapsed_time()).count();
        auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
        SDL_SetWindowTitle(window, title.c_str());
      } else {
        SDL_SetWindowTitle(window, "nes-emulator | Paused");
      }

      elapsed_frames = 0;
      fps_timer.restart();
    }

    if (running) {
      get_controllers();
      update_controllers();
      CPU::get().run_frame();

      get_samples();
      update_frame();

      ++elapsed_frames;
    }

    render();
  }
}
}  // namespace nes
