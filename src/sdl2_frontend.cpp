#include "sdl2_frontend.h"

#include <fmt/format.h>

#include <nes/Emulator.h>
#include <timer.h>

using namespace std::chrono;
using namespace std::chrono_literals;

SDL_Scancode PAUSE          = SDL_SCANCODE_ESCAPE;
SDL_Scancode RESET          = SDL_SCANCODE_R;
SDL_Scancode SAVE_SNAPSHOT  = SDL_SCANCODE_F1;
SDL_Scancode LOAD_SNAPSHOT  = SDL_SCANCODE_F3;
SDL_Scancode TOGGLE_LIMITER = SDL_SCANCODE_TAB;

SDL_Scancode VOLUME_UP   = SDL_SCANCODE_KP_PLUS;
SDL_Scancode VOLUME_DOWN = SDL_SCANCODE_KP_MINUS;

SDL_Scancode KEY_A[2]      = {SDL_SCANCODE_A, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_B[2]      = {SDL_SCANCODE_S, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_SELECT[2] = {SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_START[2]  = {SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_UP[2]     = {SDL_SCANCODE_UP, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_DOWN[2]   = {SDL_SCANCODE_DOWN, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_LEFT[2]   = {SDL_SCANCODE_LEFT, SDL_SCANCODE_ESCAPE};
SDL_Scancode KEY_RIGHT[2]  = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_ESCAPE};

namespace nes {
sdl2_frontend::sdl2_frontend(int argc, char* argv[]) : args(argv, argv + argc)
{
  if (argc == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  util::file_manager::get().setup();
  util::file_manager::get().set_rom(argv[1]);

  // nes::util::fmngr.set_rom(nes::util::fmngr.get_app_path() /
  // "../roms/smb3.nes");

  // std::ofstream log_file{app_path / "nes-emulator.log"};
  // lib::log::get().set_stream(log_file);
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
  controller::get().update_state(0, controller_state[0]);
}

void sdl2_frontend::update_frame()
{
  SDL_UpdateTexture(texture, nullptr, ppu::get().get_back_buffer().data(),
                    Emulator::width * sizeof(std::uint32_t));
}

void sdl2_frontend::render()
{
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void sdl2_frontend::get_samples()
{
  if (apu::get().samples_available(audio_buffer.size())) {
    auto sample_count =
        apu::get().get_samples(audio_buffer.data(), audio_buffer.size());
    sound_queue->write(audio_buffer.data(), sample_count);
  }
}

void sdl2_frontend::process_keypress(SDL_KeyboardEvent& key_event)
{
  auto key = key_event.keysym.scancode;

  if (key == PAUSE) {
    running = !running;
  } else if (key == RESET) {
    cpu::get().reset();
    ppu::get().reset();
  } else if (key == TOGGLE_LIMITER) {
    fps_limiter = !fps_limiter;
  } else if (key == SAVE_SNAPSHOT) {
    Emulator::get().save_snapshot();
  } else if (key == LOAD_SNAPSHOT) {
    Emulator::get().load_snapshot();
  } else if (key == VOLUME_UP) {
    volume = std::min(1.0, volume + 0.1);
    apu::get().volume(volume);
  } else if (key == VOLUME_DOWN) {
    volume = std::max(0.0, volume - 0.1);
    apu::get().volume(volume);
  }
}

void sdl2_frontend::run()
{
  Emulator::get().power_on();

  running = true;
  apu::get().volume(volume);

  lib::timer fps_timer;

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: cartridge::get().dump_prg_ram(); return;
        case SDL_KEYDOWN: process_keypress(e.key); break;
      }
    }

    if (fps_timer.elapsed_time() > 1s) {
      if (running) {
        auto fps =
            elapsed_frames / duration<double>(fps_timer.elapsed_time()).count();
        auto title = fmt::format("nes-emulator | {:5.2f}fps", fps);
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
      cpu::get().run_frame();

      get_samples();
      update_frame();

      ++elapsed_frames;
    }

    render();
  }
}
}  // namespace nes
