#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>

#include <nes/emulator.h>

// todo: reimplement this
#include <Sound_Queue.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_KeyboardEvent;

namespace nes {
class sdl2_frontend {
public:
  sdl2_frontend(int, char*[]);
  ~sdl2_frontend();

  sdl2_frontend(const sdl2_frontend&) = delete;
  sdl2_frontend(sdl2_frontend&&)      = delete;
  sdl2_frontend& operator=(const sdl2_frontend&) = delete;
  sdl2_frontend& operator=(sdl2_frontend&&) = delete;

  void init();
  void run();

private:
  void run_emulation();
  void render();
  void get_controllers();
  void update_controllers();
  void update_frame();
  void get_samples();

  void process_keypress(SDL_KeyboardEvent&);

  emulator emu;

  std::vector<std::string_view> args;

  static constexpr int width  = 256;
  static constexpr int height = 240;

  double volume = 0.1;

  std::thread emulation_thread;

  std::atomic<bool> frame_ready    = false;
  std::atomic<bool> running        = false;
  std::atomic<bool> fps_limiter    = true;
  std::atomic<int>  elapsed_frames = 0;

  std::array<std::atomic<std::uint8_t>, 2> controller_state{};

  //
  // Pending events
  //

  std::atomic<bool> pending_exit          = false;
  std::atomic<bool> pending_reset         = false;
  std::atomic<bool> pending_save_snapshot = false;
  std::atomic<bool> pending_load_snapshot = false;
  std::atomic<bool> pending_volume_up     = false;
  std::atomic<bool> pending_volume_down   = false;

  SDL_Window*         window;
  SDL_Renderer*       renderer;
  SDL_Texture*        texture;
  const std::uint8_t* keys;

  // todo: reimplement this
  std::array<std::int16_t, 4096> audio_buffer{};
  std::unique_ptr<Sound_Queue>   sound_queue;
};
}  // namespace nes
