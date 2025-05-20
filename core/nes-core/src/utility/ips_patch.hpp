#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "lib/common.hpp"

namespace nes::utility {
class IpsPatch final {
public:
  explicit IpsPatch(const std::filesystem::path& path);

  [[nodiscard]] auto patch(const std::vector<u8>& rom) -> std::vector<u8>;

private:
  [[nodiscard]] auto check() -> bool;
  void build();
  [[nodiscard]] auto read_record() -> bool;

  struct RecordEntry {
    u32 addr = 0;
    u16 length = 0;
    std::vector<u8> data;
  };

  std::vector<RecordEntry> records;
  std::ifstream ips_file;
  usize min_size = 0;
};
} // namespace nes::utility
