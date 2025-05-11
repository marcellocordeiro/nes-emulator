#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "lib/common.hpp"

static auto read_binary_file(const std::filesystem::path& path) -> std::vector<u8> {
  std::ifstream stream(path, std::ios::binary);

  stream.seekg(0, std::ios_base::end);
  const auto length = stream.tellg();
  stream.seekg(0, std::ios_base::beg);

  std::vector file(length, u8{});
  stream.read(reinterpret_cast<char*>(file.data()), length);

  return file;
}
