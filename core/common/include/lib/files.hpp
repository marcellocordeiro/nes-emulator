#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "common.hpp"

namespace lib {
inline auto read_binary_file(const std::filesystem::path& path) -> std::vector<u8> {
  std::ifstream stream(path, std::ios::binary);

  const auto length = std::filesystem::file_size(path);

  std::vector file(length, u8{});

  const auto stream_size = static_cast<std::streamsize>(length);
  stream.read(reinterpret_cast<char*>(file.data()), stream_size);

  return file;
}
} // namespace lib
