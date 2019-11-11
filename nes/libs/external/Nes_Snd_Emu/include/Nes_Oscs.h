// Private oscillators used by Nes_Apu

// Nes_Snd_Emu 0.1.7. Copyright (C) 2003-2005 Shay Green. GNU LGPL license.

#pragma once

#include "Blip_Buffer.h"

class Nes_Apu;

struct Nes_Osc {
  unsigned char regs[4];
  bool          reg_written[4];
  Blip_Buffer*  output;
  int           length_counter;  // length counter (0 if unused by oscillator)
  int           delay    = 0;    // delay until next (potential) transition
  int           last_amp = 0;    // last amplitude oscillator was outputting

  void clock_length(int halt_mask);
  int  period() const { return (regs[3] & 7) * 0x100 + (regs[2] & 0xff); }

  int update_amp(int amp)
  {
    int delta = amp - last_amp;
    last_amp  = amp;
    return delta;
  }
};

struct Nes_Envelope : Nes_Osc {
  int envelope  = 0;
  int env_delay = 0;

  void clock_envelope();
  int  volume() const;
};

// Nes_Square
struct Nes_Square : Nes_Envelope {
  enum { negate_flag = 0x08 };
  enum { shift_mask = 0x07 };
  enum { phase_range = 8 };
  int phase       = 0;
  int sweep_delay = 0;

  typedef Blip_Synth<blip_good_quality, 15> Synth;
  const Synth* synth = nullptr;  // shared between squares

  void clock_sweep(int adjust);
  void run(cpu_time_t, cpu_time_t);
};

// Nes_Triangle
struct Nes_Triangle : Nes_Osc {
  enum { phase_range = 16 };
  int                               phase          = phase_range;
  int                               linear_counter = 0;
  Blip_Synth<blip_good_quality, 15> synth;

  int  calc_amp() const;
  void run(cpu_time_t, cpu_time_t);
  void clock_linear_counter();
};

// Nes_Noise
struct Nes_Noise : Nes_Envelope {
  int                              noise = 1 << 14;
  Blip_Synth<blip_med_quality, 15> synth;

  void run(cpu_time_t, cpu_time_t);
};

// Nes_Dmc
struct Nes_Dmc : Nes_Osc {
  int  address     = 0;  // address of next byte to read
  int  period      = 0x036;
  int  buf         = 0;
  int  bits_remain = 1;
  int  bits        = 0;
  bool buf_empty   = true;
  bool silence     = true;

  enum { loop_flag = 0x40 };

  int dac = 0;

  int (*rom_reader)(void*, cpu_addr_t) =
      nullptr;  // needs to be initialized to rom read function
  void* rom_reader_data;

  Nes_Apu* apu;

  Blip_Synth<blip_med_quality, 127> synth;

  void start();
  void write_register(int, int);
  void run(cpu_time_t, cpu_time_t);
  void fill_buffer();
  void reload_sample();
  int  count_reads(cpu_time_t, cpu_time_t*) const;
};
