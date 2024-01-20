#pragma once

#include <filesystem>
#include <vector>

namespace lib {
auto load_bytes_from_file(const std::filesystem::path& path)
  -> std::vector<uint8_t>;
}  // namespace lib
