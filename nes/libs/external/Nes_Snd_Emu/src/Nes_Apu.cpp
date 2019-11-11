// Nes_Snd_Emu 0.1.7. http://www.slack.net/~ant/libs/

#include <cassert>

#include "Nes_Apu.h"

/* Copyright (C) 2003-2005 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details. You should have received a copy of the GNU Lesser General
Public License along with this module; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

Nes_Apu::Nes_Apu()
{
  dmc.apu       = this;
  square1.synth = &square_synth;
  square2.synth = &square_synth;

  oscs[0] = &square1;
  oscs[1] = &square2;
  oscs[2] = &triangle;
  oscs[3] = &noise;
  oscs[4] = &dmc;

  output(nullptr);
  volume(1.0);

  write_register(0, 0x4017, 0x00);
  write_register(0, 0x4015, 0x00);

  for (cpu_addr_t addr = start_addr; addr <= 0x4013; addr++) {
    write_register(0, addr, (addr & 3) ? 0x00 : 0x10);
  }
}

void Nes_Apu::volume(double v)
{
  square_synth.volume(0.1128 * v);
  triangle.synth.volume(0.12765 * v);
  noise.synth.volume(0.0741 * v);
  dmc.synth.volume(0.42545 * v);
}

void Nes_Apu::output(Blip_Buffer* buffer)
{
  for (int i = 0; i < osc_count; i++) osc_output(i, buffer);
}

// frames

void Nes_Apu::run_until(cpu_time_t end_time)
{
  assert(end_time >= last_time);

  if (end_time == last_time) return;

  while (true) {
    // earlier of next frame time or end time
    cpu_time_t time = last_time + frame_delay;
    if (time > end_time) time = end_time;
    frame_delay -= time - last_time;

    // run oscs to present
    square1.run(last_time, time);
    square2.run(last_time, time);
    triangle.run(last_time, time);
    noise.run(last_time, time);
    dmc.run(last_time, time);
    last_time = time;

    if (time == end_time) break;  // no more frames to run

    // take frame-specific actions
    frame_delay = frame_period;
    switch (frame++) {
      case 0:
        // fall through
      case 2:
        // clock length and sweep on frames 0 and 2
        square1.clock_length(0x20);
        square2.clock_length(0x20);
        noise.clock_length(0x20);
        triangle.clock_length(0x80);  // different bit for halt flag on triangle

        square1.clock_sweep(-1);
        square2.clock_sweep(0);
        break;

      case 1:
        // frame 1 is slightly shorter
        frame_delay -= 2;
        break;

      case 3:
        frame = 0;

        // frame 3 is almost twice as long in mode 1
        if (frame_mode & 0x80) frame_delay += frame_period - 6;
        break;
    }

    // clock envelopes and linear counter every frame
    triangle.clock_linear_counter();
    square1.clock_envelope();
    square2.clock_envelope();
    noise.clock_envelope();
  }
}

void Nes_Apu::end_frame(cpu_time_t end_time)
{
  if (end_time > last_time) run_until(end_time);

  // make times relative to new frame
  last_time -= end_time;
  assert(last_time >= 0);
}

// registers

static const unsigned char length_table[0x20] = {
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C,
    0x0A, 0x0E, 0x0C, 0x1A, 0x0E, 0x0C, 0x10, 0x18, 0x12, 0x30, 0x14,
    0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E};

void Nes_Apu::write_register(cpu_time_t time, cpu_addr_t addr, int data)
{
  assert(addr > 0x20);  // addr must be actual address (i.e. 0x40xx)
  assert((unsigned)data <= 0xff);

  // Ignore addresses outside range
  if (addr < start_addr || end_addr < addr) return;

  run_until(time);

  if (addr < 0x4014) {
    // Write to channel
    int      osc_index = (addr - start_addr) >> 2;
    Nes_Osc* osc       = oscs[osc_index];

    int reg               = addr & 3;
    osc->regs[reg]        = data;
    osc->reg_written[reg] = true;

    if (osc_index == 4) {
      // handle DMC specially
      dmc.write_register(reg, data);
    } else if (reg == 3) {
      // load length counter
      if ((osc_enables >> osc_index) & 1)
        osc->length_counter = length_table[(data >> 3) & 0x1f];

      // reset square phase
      if (osc_index < 2)
        ((Nes_Square*)osc)->phase = Nes_Square::phase_range - 1;
    }
  } else if (addr == 0x4015) {
    // Channel enables
    for (int i = osc_count; i--;)
      if (!((data >> i) & 1)) oscs[i]->length_counter = 0;

    int old_enables = osc_enables;
    osc_enables     = data;
    if (!(data & 0x10)) {
    } else if (!(old_enables & 0x10)) {
      dmc.start();  // dmc just enabled
    }

  } else if (addr == 0x4017) {
    // Frame mode
    frame_mode = data;

    // mode 1
    frame_delay = (frame_delay & 1);
    frame       = 0;

    if (!(data & 0x80)) {
      // mode 0
      frame = 1;
      frame_delay += frame_period;
    }
  }
}

int Nes_Apu::read_status(cpu_time_t time)
{
  run_until(time - 1);

  int result = 0;

  for (int i = 0; i < osc_count; i++)
    if (oscs[i]->length_counter) result |= 1 << i;

  run_until(time);

  return result;
}

void Nes_Apu::osc_output(int osc, Blip_Buffer* buf)
{
  assert(0 <= osc &&
         osc < osc_count);  // Nes_Apu::osc_output(): Index out of range
  oscs[osc]->output = buf;
}

void Nes_Apu::dmc_reader(int (*func)(void*, cpu_addr_t), void* user_data)
{
  dmc.rom_reader_data = user_data;
  dmc.rom_reader      = func;
}
