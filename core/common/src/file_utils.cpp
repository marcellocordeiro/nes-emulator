#include "file_utils.h"

#include <fstream>

namespace lib {
auto load_bytes_from_file(const std::filesystem::path& path)
  -> std::vector<uint8_t> {
  std::basic_ifstream<uint8_t> file(path, std::ios::binary);

  return {std::istreambuf_iterator<uint8_t>{file}, {}};
}
}  // namespace lib
