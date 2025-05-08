#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "lib/common.hpp"

static auto read_binary_file(const std::filesystem::path& path) -> std::vector<u8> {
  std::ifstream stream(path, std::ios::binary);

  std::vector file(std::filesystem::file_size(path), u8{});
  stream.read(reinterpret_cast<char*>(file.data()), file.size());

  return file;
}
