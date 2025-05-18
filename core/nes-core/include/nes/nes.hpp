#pragma once

#include <filesystem>

#include "lib/common.hpp"

namespace nes {
class Nes {
public:
  void set_app_path(const std::filesystem::path&);
  void load(const std::filesystem::path&);
  void reset();
  void power_on();
  void power_off();

  void run_frame();
  auto get_frame_buffer() -> const u32*;

  void update_controller_state(usize, u8);
};
} // namespace nes
