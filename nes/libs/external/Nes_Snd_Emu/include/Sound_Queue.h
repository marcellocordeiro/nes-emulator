#pragma once

#include <cstddef>
#include <cstdint>

// Simple sound queue for synchronous sound handling in SDL
// Simple SDL sound wrapper that has a synchronous interface
// Copyright (C) 2005 Shay Green. MIT license.

struct SDL_semaphore;

class Sound_Queue {
public:
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

  sample_t* volatile bufs          = nullptr;
  SDL_semaphore* volatile free_sem = nullptr;
  int volatile read_buf            = 0;
  int write_buf                    = 0;
  int write_pos                    = 0;
};
