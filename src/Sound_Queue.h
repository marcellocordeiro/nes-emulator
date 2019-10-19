#pragma once

// Simple sound queue for synchronous sound handling in SDL
// Simple SDL sound wrapper that has a synchronous interface
// Copyright (C) 2005 Shay Green. MIT license.

#include <atomic>
#include <cstdint>

#include <SDL_mutex.h>

class Sound_Queue {
public:
  using size_t  = std::size_t;
  using uint8_t = std::uint8_t;

  Sound_Queue() = default;
  ~Sound_Queue();

  typedef short sample_t;

  void init();

  // Number of samples in buffer waiting to be played
  int sample_count() const;

  // Write samples to buffer and block until enough space is available
  void write(const sample_t*, int count);

  void fill_buffer(uint8_t*, int);

  static constexpr size_t buf_size  = 2048;
  static constexpr size_t buf_count = 3;

private:
  sample_t* buf(int index);

  sample_t*        bufs      = nullptr;
  SDL_semaphore*   free_sem  = nullptr;
  std::atomic<int> read_buf  = 0;
  int              write_buf = 0;
  int              write_pos = 0;
};
