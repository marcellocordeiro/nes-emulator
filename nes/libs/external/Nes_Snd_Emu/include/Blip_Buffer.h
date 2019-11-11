// Buffer of sound samples into which band-limited waveforms can be synthesized
// using Blip_Wave or Blip_Synth.

// Blip_Buffer 0.3.3. Copyright (C) 2003-2005 Shay Green. GNU LGPL license.

#pragma once

#include <cstdint>

// Source time unit.
typedef long blip_time_t;

// Type of sample produced. Signed 16-bit format.
typedef int16_t blip_sample_t;

// Make buffer as large as possible (currently about 65000 samples)
inline constexpr int blip_default_length = 0;

class Blip_Buffer {
public:
  // Construct an empty buffer.
  Blip_Buffer() = default;
  ~Blip_Buffer();

  // Set output sample rate and buffer length in milliseconds (1/1000 sec),
  // then clear buffer. If length is not specified, make as large as possible.
  void sample_rate(long samples_per_sec, int msec_length = blip_default_length);
  // to do: rename to set_sample_rate

  // Number of source time units per second
  void clock_rate(long);

  // Set frequency at which high-pass filter attenuation passes -3dB
  void bass_freq(int frequency);

  // Remove all available samples and clear buffer to silence. If
  // 'entire_buffer' is false, just clear out any samples waiting rather than
  // the entire buffer.
  void clear(bool entire_buffer = true);

  // to do:
  // Notify Blip_Buffer that synthesis has been performed until specified time
  // void run_until( blip_time_t );

  // End current time frame of specified duration and make its samples available
  // (along with any still-unread samples) for reading with read_samples().
  // Begin a new time frame at the end of the current frame. All transitions
  // must have been added before 'time'.
  void end_frame(blip_time_t time);

  // Number of samples available for reading with read_samples()
  long samples_avail() const;

  // Read at most 'max_samples' out of buffer into 'dest', removing them from
  // from the buffer. Return number of samples actually read and removed. If
  // stereo is true, increment 'dest' one extra time after writing each sample,
  // to allow easy interleving of two channels into a stereo output buffer.
  long read_samples(blip_sample_t* dest, long max_samples, bool stereo = false);

  // Remove 'count' samples from those waiting to be read
  void remove_samples(long count);

  // not documented yet

  void remove_silence(long count);

  typedef unsigned long resampled_time_t;

  resampled_time_t resampled_time(blip_time_t t) const
  {
    return t * factor_ + offset_;
  }

  resampled_time_t resampled_duration(int t) const { return t * factor_; }

private:
  // noncopyable
  Blip_Buffer(const Blip_Buffer&);
  Blip_Buffer& operator=(const Blip_Buffer&);

  // Don't use the following members. They are public only for technical
  // reasons.
public:
  enum { widest_impulse_ = 24 };
  typedef uint16_t buf_t_;

  unsigned long    factor_      = ~0ul;
  resampled_time_t offset_      = 0;
  buf_t_*          buffer_      = nullptr;
  unsigned         buffer_size_ = 0;

private:
  long reader_accum    = 0;
  int  bass_shift      = 0;
  long samples_per_sec = 44100;
  long clocks_per_sec  = 0;
  int  bass_freq_      = 16;
  int  length_         = 0;

  enum { accum_fract = 15 };  // less than 16 to give extra sample range
  enum {
    sample_offset = 0x7F7F
  };  // repeated byte allows memset to clear buffer
};

// Low-pass equalization parameters (see notes.txt)
class blip_eq_t {
public:
  blip_eq_t() = default;
  blip_eq_t(double treble, long cutoff, long sample_rate);

private:
  double treble;
  long   cutoff;
  long   sample_rate;
  friend class Blip_Impulse_;
};

// End of public interface

inline constexpr int BLIP_BUFFER_ACCURACY = 16;
inline constexpr int blip_res_bits_       = 5;

typedef uint32_t blip_pair_t_;

class Blip_Impulse_ {
  typedef uint16_t imp_t;

  blip_eq_t eq;
  double    volume_unit_;
  imp_t*    impulses;
  imp_t*    impulse;
  int       width;
  int       fine_bits;
  int       res;
  bool      generate;

  void fine_volume_unit();
  void scale_impulse(int unit, imp_t*) const;

public:
  uint32_t offset;

  void init(blip_pair_t_* impulses, int width, int res, int fine_bits = 0);
  void volume_unit(double);
  void treble_eq(const blip_eq_t&);
};

#include "Blip_Synth.h"