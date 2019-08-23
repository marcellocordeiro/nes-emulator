#pragma once

#include <array>

// Simple sound queue for synchronous sound handling in SDL

// Copyright (C) 2005 Shay Green. MIT license.

// Simple SDL sound wrapper that has a synchronous interface

struct SDL_semaphore;

class Sound_Queue {
public:
  Sound_Queue();
  ~Sound_Queue();

  // Initialize with specified sample rate and channel count.
  void init(long sample_rate, int chan_count = 1);

  // Number of samples in buffer waiting to be played
  int sample_count() const;

  // Write samples to buffer and block until enough space is available
  typedef short sample_t;
  void          write(const sample_t*, int count);

private:
  enum { buf_size = 2048 };
  enum { buf_count = 3 };

  std::array<sample_t, buf_size * buf_count> bufs;

  SDL_semaphore* free_sem;
  int      read_buf;
  int      write_buf;
  int      write_pos;
  bool     sound_open;

  sample_t* buf(int index);
  void      fill_buffer(uint8_t*, int);
};
