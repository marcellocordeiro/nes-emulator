#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "lib/common.h"

namespace nes::utility {
class IpsPatch final {
public:
  explicit IpsPatch(const std::filesystem::path&);

  auto patch(const std::vector<u8>&) -> std::vector<u8>;

private:
  auto check() -> bool;
  void build();
  auto read_record() -> bool;

  struct record_entry {
    u32 addr = 0;
    u16 length = 0;
    std::vector<u8> data;
  };

  std::vector<record_entry> records;
  std::ifstream ips_file;
  usize min_size = 0;
};
} // namespace nes::utility
