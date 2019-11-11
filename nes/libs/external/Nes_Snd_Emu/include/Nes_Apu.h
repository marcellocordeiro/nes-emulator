// NES 2A03 APU sound chip emulator

// Nes_Snd_Emu 0.1.7. Copyright (C) 2003-2005 Shay Green. GNU LGPL license.

#pragma once

#include <climits>
#include <cstdint>

typedef long     cpu_time_t;  // CPU clock cycle count
typedef uint16_t cpu_addr_t;  // 16-bit memory address

#include "Nes_Oscs.h"

class Nes_Apu {
public:
  Nes_Apu();

  // Set buffer to generate all sound into, or disable sound if NULL
  void output(Blip_Buffer*);

  // Set memory reader callback used by DMC oscillator to fetch samples.
  // When callback is invoked, 'user_data' is passed unchanged as the
  // first parameter.
  void dmc_reader(int (*callback)(void* user_data, cpu_addr_t),
                  void* user_data = nullptr);

  // All time values are the number of CPU clock cycles relative to the
  // beginning of the current time frame. Before resetting the CPU clock
  // count, call end_frame( last_cpu_time ).

  // Write to register (0x4000-0x4017, except 0x4014 and 0x4016)
  enum { start_addr = 0x4000 };
  enum { end_addr = 0x4017 };
  void write_register(cpu_time_t, cpu_addr_t, int data);

  // Read from status register at 0x4015
  int read_status(cpu_time_t);

  // Run all oscillators up to specified time, end current time frame, then
  // start a new time frame at time 0. Time frames have no effect on emulation
  // and each can be whatever length is convenient.
  void end_frame(cpu_time_t);

  // Set overall volume (default is 1.0)
  void volume(double);

  // Set sound output of specific oscillator to buffer. If buffer is NULL,
  // the specified oscillator is muted and emulation accuracy is reduced.
  // The oscillators are indexed as follows: 0) Square 1, 1) Square 2,
  // 2) Triangle, 3) Noise, 4) DMC.
  enum { osc_count = 5 };
  void osc_output(int index, Blip_Buffer* buffer);

  // Run APU until specified time, so that any DMC memory reads can be
  // accounted for (i.e. inserting CPU wait states).
  void run_until(cpu_time_t);

  // End of public interface.
private:
  // noncopyable
  Nes_Apu(const Nes_Apu&);
  Nes_Apu& operator=(const Nes_Apu&);

  Nes_Osc*     oscs[osc_count];
  Nes_Square   square1;
  Nes_Square   square2;
  Nes_Noise    noise;
  Nes_Triangle triangle;
  Nes_Dmc      dmc;

  cpu_time_t last_time    = 0;  // has been run until this time in current frame
  int        frame_period = 7458;
  int        frame_delay  = 1;  // cycles until frame counter runs next
  int        frame;             // current frame (0-3)
  int        osc_enables = 0;
  int        frame_mode;

  Nes_Square::Synth square_synth;  // shared by squares

  friend struct Nes_Dmc;
};