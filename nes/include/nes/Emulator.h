#pragma once

#include <array>
#include <filesystem>

class Emulator {
public:
  Emulator(const Emulator&) = delete;
  Emulator(Emulator&&)      = delete;
  Emulator& operator=(const Emulator&) = delete;
  Emulator& operator=(Emulator&&) = delete;

  static void set_app_path(const std::filesystem::path&);
  static void load(const std::filesystem::path&);
  static void power_on();
  static void reset();

  //
  // Component access
  //

  static void            run_frame();
  static const uint32_t* get_back_buffer();
  static bool            samples_available(size_t);
  static long            get_audio_samples(std::array<std::int16_t, 4096>&);
  static void            volume(double);

  static void update_controller_state(size_t, uint8_t);

  //
  // Snapshot
  //

  static void save_snapshot();
  static void load_snapshot();

  static constexpr int width  = 256;
  static constexpr int height = 240;

  static constexpr const char* title = "nes-emulator";

private:
  Emulator() = default;
};
