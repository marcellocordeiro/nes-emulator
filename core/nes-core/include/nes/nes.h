#pragma once

#include <filesystem>
#include <string_view>

#include "lib/common.h"

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
  static auto get_frame_buffer() -> const u32*;

  static void update_controller_state(usize, u8);

  //
  // Snapshot
  //

  static void save_snapshot();
  static void load_snapshot();

  static constexpr i32 SCREEN_WIDTH = 256;
  static constexpr i32 SCREEN_HEIGHT = 240;
  static constexpr i32 FRAMEBUFFER_SIZE = (SCREEN_WIDTH * SCREEN_HEIGHT) * sizeof(u32);

  static constexpr i32 width = SCREEN_WIDTH;
  static constexpr i32 height = SCREEN_HEIGHT;

  static constexpr auto title = std::string_view("nes-emulator");

private:
  Nes() = default;
};
