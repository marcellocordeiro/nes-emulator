#include "io.h"

#include <algorithm>
#include <string>

#include <SDL.h>
#include <fmt/format.h>

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "emulator.h"
#include "ppu.h"
#include "timer.h"

// todo: reimplement this
#include "Sound_Queue.h"

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

namespace SDL2 {
Instance::Instance()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

Instance::~Instance()
{
  SDL_Quit();
}

void Deleter::operator()(SDL_Window* ptr)
{
  if (ptr) {
    SDL_DestroyWindow(ptr);
  }
}

void Deleter::operator()(SDL_Renderer* ptr)
{
  if (ptr) {
    SDL_DestroyRenderer(ptr);
  }
}

void Deleter::operator()(SDL_Texture* ptr)
{
  if (ptr) {
    SDL_DestroyTexture(ptr);
  }
}
}  // namespace SDL2

namespace nes {
io::io(emulator& emu_ref) : emu(emu_ref) {}
// io::~io() = default;
io::~io()
{
  if (sound_open) {
    SDL_PauseAudio(true);
    SDL_CloseAudio();
  }

  pending_exit = true;
  cpu_thread.join();
  emu.get_cartridge()->dump_prg_ram();
}

void io::init()
{
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window.reset(SDL_CreateWindow(
      "nes-emu",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width * 2,
      height * 2,
      0));

  renderer.reset(
      SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

  SDL_RenderSetLogicalSize(renderer.get(), width, height);

  texture.reset(SDL_CreateTexture(
      renderer.get(),
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      width,
      height));

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
  sound_open = true;
}

void io::close()
{
  // leak?
  // valgrind --log-file='valgrind%p.log' --track-origins=yes --leak-check=full
  // ./nes-emu

  // texture  = nullptr;
  // renderer = nullptr;
  // window   = nullptr;
  // SDL_Quit();
}

uint8_t io::get_controller(size_t n) const
{
  return controller_state[n];
}

void io::update_controllers()
{
  for (size_t i = 0; i < 2; ++i) {
    controller_state[i] = 0;
    controller_state[i] |= (keys[KEY_A[i]]) << 0u;
    controller_state[i] |= (keys[KEY_B[i]]) << 1u;
    controller_state[i] |= (keys[KEY_SELECT[i]]) << 2u;
    controller_state[i] |= (keys[KEY_START[i]]) << 3u;
    controller_state[i] |= (keys[KEY_UP[i]]) << 4u;
    controller_state[i] |= (keys[KEY_DOWN[i]]) << 5u;
    controller_state[i] |= (keys[KEY_LEFT[i]]) << 6u;
    controller_state[i] |= (keys[KEY_RIGHT[i]]) << 7u;
  }
}

void io::update_frame(const std::array<uint32_t, 256 * 240>& back_buffer)
{
  //std::lock_guard<std::mutex> render_guard(render_lock);
  // front_buffer.swap(back_buffer);
  //front_buffer = back_buffer;
  SDL_UpdateTexture(
        texture.get(), nullptr, back_buffer.data(), width * sizeof(uint32_t));
}

void io::render()
{
  {
    //std::lock_guard<std::mutex> render_guard(render_lock);
    //SDL_UpdateTexture(texture.get(), nullptr, front_buffer.data(), width * sizeof(uint32_t));
  }
  SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
}

void io::write_samples(int16_t* buffer, long samples)
{
  sound_queue->write(buffer, samples);
}

void io::process_keypress(SDL_KeyboardEvent& key_event)
{
  auto key = key_event.keysym.scancode;

  if (key == PAUSE) {
    running = !running;
  } else if (key == RESET) {
    pending_reset = true;
  } else if (key == TOGGLE_LIMITER) {
    fps_limiter = !fps_limiter;
  } else if (key == SAVE_SNAPSHOT) {
    pending_save_snapshot = true;
  } else if (key == LOAD_SNAPSHOT) {
    pending_load_snapshot = true;
  } else if (key == VOLUME_UP) {
    pending_volume_up = true;
  } else if (key == VOLUME_DOWN) {
    pending_volume_down = true;
  }
}

void io::run_cpu()
{
  using namespace std::chrono;
  using namespace std::chrono_literals;

  // todo: find a better way to limit the frame rate
  constexpr auto frame_time =
      round<steady_clock::duration>(duration<long long, std::ratio<1, 60>>{1});
  auto frame_begin = steady_clock::now();
  auto frame_end   = frame_begin + frame_time;

  while (true) {
    if (pending_exit) {
      return;
    } else if (pending_reset) {
      emu.get_cpu()->reset();
      emu.get_ppu()->reset();
      pending_reset = false;
    } else if (pending_load_snapshot) {
      emu.load_snapshot();
      pending_load_snapshot = false;
    } else if (pending_save_snapshot) {
      emu.save_snapshot();
      pending_save_snapshot = false;
    } else if (pending_volume_up) {
      volume = std::min(1.0, volume + 0.1);
      emu.get_apu()->volume(volume);
      pending_volume_up = false;
    } else if (pending_volume_down) {
      volume = std::max(0.0, volume - 0.1);
      emu.get_apu()->volume(volume);
      pending_volume_down = false;
    }

    if (running) {
      emu.get_controller()->update_state(0, controller_state[0]);
      emu.get_cpu()->run_frame();
      update_frame(emu.get_ppu()->get_back_buffer());
      ++elapsed_frames;
    }

    if (fps_limiter) std::this_thread::sleep_until(frame_end);
    render();

    frame_begin = frame_end;
    frame_end   = frame_begin + frame_time;
  }
}

void io::run()
{
  running     = true;
  emu.get_apu()->volume(volume);

  using namespace std::chrono;
  using namespace std::chrono_literals;

  cpu_thread = std::thread(&io::run_cpu, this);

  // todo: find a better way to limit the frame rate
  constexpr auto frame_time =
      round<steady_clock::duration>(duration<long long, std::ratio<1, 60>>{1});
  auto frame_begin = steady_clock::now();
  auto frame_end   = frame_begin + frame_time;

  lib::timer fps_timer;

  while (true) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: pending_exit = true; return;
        case SDL_KEYDOWN: process_keypress(e.key); break;
      }
    }

    update_controllers();

    if (fps_timer.elapsed_time() > 1s) {
      if (running) {
        auto fps =
            elapsed_frames / duration<double>(fps_timer.elapsed_time()).count();
        auto title = fmt::format("[{:5.2f}fps] - nes-emu", fps);
        SDL_SetWindowTitle(window.get(), title.c_str());
      } else {
        SDL_SetWindowTitle(window.get(), "[paused] - nes-emu");
      }

      elapsed_frames = 0;
      fps_timer.restart();
    }

    std::this_thread::sleep_until(frame_end);

    frame_begin = frame_end;
    frame_end   = frame_begin + frame_time;
  }
}
}  // namespace nes
