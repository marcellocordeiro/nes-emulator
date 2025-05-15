#pragma once

#include <filesystem>
#include <string_view>

#include "lib/common.hpp"

namespace nes {
class Nes {
public:
  void set_app_path(const std::filesystem::path&);
  void load(const std::filesystem::path&);
  void reset();
  void power_on();
  void power_off();

  //
  // Component access
  //

  void run_frame();
  auto get_frame_buffer() -> const u32*;

  void update_controller_state(usize, u8);

  //
  // Snapshot
  //

  void save_snapshot();
  void load_snapshot();

  static constexpr i32 SCREEN_WIDTH = 256;
  static constexpr i32 SCREEN_HEIGHT = 240;
  static constexpr i32 FRAMEBUFFER_SIZE = (SCREEN_WIDTH * SCREEN_HEIGHT) * sizeof(u32);

  static constexpr i32 width = SCREEN_WIDTH;
  static constexpr i32 height = SCREEN_HEIGHT;

  static constexpr auto title = std::string_view("nes-emulator");
};
} // namespace nes
