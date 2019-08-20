#include "io.h"

#include <algorithm>
#include <string>

#include <fmt/format.h>

#include "apu.h"
#include "cartridge.h"
#include "cpu.h"
#include "emulator.h"
#include "ppu.h"
#include "timer.h"

// todo: reimplement this
#include "Sound_Queue.h"

namespace nes {
io::io(nes::emulator& emulator_ref) : emulator(emulator_ref) {}
io::~io() = default;

void io::start()
{
  // SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window.reset(SDL_CreateWindow(
      "nes-emulator",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width * 2,
      height * 2,
      0));

  renderer.reset(SDL_CreateRenderer(
      window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  SDL_RenderSetLogicalSize(renderer.get(), width, height);

  texture.reset(SDL_CreateTexture(
      renderer.get(),
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      width,
      height));

  keys = SDL_GetKeyboardState(nullptr);

  sound_queue = std::make_unique<Sound_Queue>();
  sound_queue->init(44100);
}

void io::close()
{
  // leak?
  // valgrind --log-file='valgrind%p.log' --track-origins=yes --leak-check=full
  // ./nes-emulator

  // texture  = nullptr;
  // renderer = nullptr;
  // window   = nullptr;
  // SDL_Quit();
}

uint8_t io::get_controller(size_t n) const
{
  uint8_t state = 0;

  state |= (keys[KEY_A[n]]) << 0u;
  state |= (keys[KEY_B[n]]) << 1u;
  state |= (keys[KEY_SELECT[n]]) << 2u;
  state |= (keys[KEY_START[n]]) << 3u;
  state |= (keys[KEY_UP[n]]) << 4u;
  state |= (keys[KEY_DOWN[n]]) << 5u;
  state |= (keys[KEY_LEFT[n]]) << 6u;
  state |= (keys[KEY_RIGHT[n]]) << 7u;

  return state;
}

void io::update_frame(std::array<uint32_t, 256 * 240>& back_buffer)
{
  while (frame_ready) {
    // Wait until the frame is rendered
    // todo: improve this
  }

  front_buffer.swap(back_buffer);
  SDL_UpdateTexture(
      texture.get(), nullptr, front_buffer.data(), width * sizeof(uint32_t));
  frame_ready = true;
}

void io::write_samples(int16_t* buffer, long samples)
{
  sound_queue->write(buffer, samples);
}

void io::render()
{
  while (true) {
    if (pending_exit) return;
    if (!frame_ready) continue;

    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
    frame_ready = false;
  }
}

void io::run()
{
  running = true;
  emulator.get_apu()->volume(volume);

  std::thread render_thread(&nes::io::render, this);

  using namespace std::chrono;
  using namespace std::chrono_literals;

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
        case SDL_QUIT:
          emulator.get_cartridge()->dump_prg_ram();
          pending_exit = true;
          render_thread.join();
          return;

        case SDL_KEYDOWN:
          if (keys[PAUSE]) {
            running = !running;
          }

          if (keys[RESET]) {
            emulator.get_cpu()->reset();
            emulator.get_ppu()->reset();
          }

          if (keys[TOGGLE_LIMITER]) {
            fps_limiter = !fps_limiter;
          }

          if (keys[SAVE_SNAPSHOT]) {
            emulator.save_snapshot();
          }

          if (keys[LOAD_SNAPSHOT]) {
            emulator.load_snapshot();
          }

          if (keys[VOLUME_UP]) {
            volume = std::min(1.0, volume + 0.1);
            emulator.get_apu()->volume(volume);
          }

          if (keys[VOLUME_DOWN]) {
            volume = std::max(0.0, volume - 0.1);
            emulator.get_apu()->volume(volume);
          }

          break;
      }
    }

    if (fps_timer.elapsed_time() > 1s) {
      if (running) {
        auto fps =
            elapsed_frames / duration<double>(fps_timer.elapsed_time()).count();
        auto title =
            fmt::format("[{:.5} fps] - nes-emulator", std::to_string(fps));
        SDL_SetWindowTitle(window.get(), title.c_str());
      } else {
        SDL_SetWindowTitle(window.get(), "[paused] - nes-emulator");
      }

      elapsed_frames = 0;
      fps_timer.restart();
    }

    if (running) {
      emulator.get_cpu()->run_frame();
      ++elapsed_frames;
    }

    if (fps_limiter) std::this_thread::sleep_until(frame_end);

    frame_begin = frame_end;
    frame_end   = frame_begin + frame_time;
  }
}
}  // namespace nes
