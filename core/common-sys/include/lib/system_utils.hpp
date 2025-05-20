#pragma once

#include <filesystem>

namespace lib {
[[nodiscard]] auto get_base_path() -> std::filesystem::path;
} // namespace lib
