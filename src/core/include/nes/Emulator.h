#pragma once

#include <array>
#include <filesystem>
#include <string_view>

class Emulator {
public:
  Emulator(const Emulator&) = delete;
  Emulator(Emulator&&)      = delete;
  auto operator=(const Emulator&) -> Emulator& = delete;
  auto operator=(Emulator&&) -> Emulator& = delete;

  static void set_app_path(const std::filesystem::path&);
  static void load(const std::filesystem::path&);
  static void reset();
  static void power_on();
  static void power_off();

  //
  // Component access
  //

  static void run_frame();
  static auto get_back_buffer() -> const uint32_t*;

  static void update_controller_state(size_t, uint8_t);

  //
  // Snapshot
  //

  static void save_snapshot();
  static void load_snapshot();

  static constexpr int width  = 256;
  static constexpr int height = 240;

  static constexpr auto title = std::string_view("nes-emulator");

private:
  Emulator() = default;
};
