#pragma once

#include <filesystem>
#include <string_view>

class Nes {
public:
  Nes(const Nes&) = delete;
  Nes(Nes&&) = delete;
  auto operator=(const Nes&) -> Nes& = delete;
  auto operator=(Nes&&) -> Nes& = delete;

  static void set_app_path(const std::filesystem::path&);
  static void load(const std::filesystem::path&);
  static void reset();
  static void power_on();
  static void power_off();

  //
  // Component access
  //

  static void run_frame();
  static auto get_frame_buffer() -> const uint32_t*;

  static void update_controller_state(size_t, uint8_t);

  //
  // Snapshot
  //

  static void save_snapshot();
  static void load_snapshot();

  static constexpr int SCREEN_WIDTH = 256;
  static constexpr int SCREEN_HEIGHT = 240;
  static constexpr int FRAMEBUFFER_SIZE = (SCREEN_WIDTH * SCREEN_HEIGHT) * sizeof(uint32_t);

  static constexpr int width = SCREEN_WIDTH;
  static constexpr int height = SCREEN_HEIGHT;

  static constexpr auto title = std::string_view("nes-emulator");

private:
  Nes() = default;
};
