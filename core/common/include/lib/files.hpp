#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "common.hpp"

namespace lib {
inline auto read_binary_file(const std::filesystem::path& path) -> std::vector<u8> {
  std::ifstream stream(path, std::ios::binary);

  stream.seekg(0, std::ios_base::end);
  const auto length = stream.tellg();
  stream.seekg(0, std::ios_base::beg);

  std::vector file(length, u8{});
  stream.read(reinterpret_cast<char*>(file.data()), length);

  return file;
}
} // namespace lib
