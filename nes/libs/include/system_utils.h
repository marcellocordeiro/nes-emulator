#pragma once

#include <filesystem>
#include <string_view>

namespace lib {
void                  hide_console();
void                  show_console();
void                  message_box(std::string_view);
std::filesystem::path get_app_path();
}  // namespace lib
