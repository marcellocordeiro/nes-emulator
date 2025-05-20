#pragma once

#include <filesystem>

#include "lib/common.hpp"

namespace nes {
class Nes {
public:
  void set_app_path(const std::filesystem::path& path);
  void load(const std::filesystem::path& path);
  void reset();
  void power_on();
  void power_off();

  void run_frame();
  auto get_frame_buffer() -> const u32*;

  void update_controller_state(usize port, u8 state);
};
} // namespace nes
