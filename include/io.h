#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include "common.h"
#include "types/forward_decl.h"

namespace SDL2 {
struct Instance {
  Instance();
  ~Instance();
};

struct Deleter {
  void operator()(SDL_Window* ptr);
  void operator()(SDL_Renderer* ptr);
  void operator()(SDL_Texture* ptr);
};

using Window   = std::unique_ptr<SDL_Window, Deleter>;
using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
using Texture  = std::unique_ptr<SDL_Texture, Deleter>;
}  // namespace SDL2

namespace nes {
class io {
public:
  io(nes::emulator&);
  ~io();

  void init();
  void run();
  void close();

  uint8_t get_controller(size_t) const;
  void    update_frame(std::array<uint32_t, 256 * 240>&);
  void    write_samples(int16_t*, long);  // todo: reimplement this
  void    render();

  void poll_events();
  void handle_keys();

private:
  void process_keypress(SDL_KeyboardEvent&);

  nes::emulator& emulator;

  static constexpr int width  = 256;
  static constexpr int height = 240;

  std::array<uint32_t, 256 * 240> front_buffer{};

  double volume = 0.1;

  std::thread       render_thread;
  std::mutex        event_lock;
  std::mutex        render_lock;
  std::atomic<bool> frame_ready    = false;
  std::atomic<bool> pending_exit   = false;
  std::atomic<bool> running        = false;
  std::atomic<bool> fps_limiter    = true;
  std::atomic<int>  elapsed_frames = 0;

  SDL2::Instance SDL2_guard;
  SDL2::Window   window;
  SDL2::Renderer renderer;
  SDL2::Texture  texture;
  const uint8_t* keys;

  // todo: reimplement this
  std::unique_ptr<Sound_Queue> sound_queue;
  bool                         sound_open = false;
};
}  // namespace nes
