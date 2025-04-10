#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include <common.h>

namespace nes::utility {
class IpsPatch final {
public:
  IpsPatch(const std::filesystem::path&);

  auto patch(const std::vector<uint8_t>&) -> std::vector<uint8_t>;

private:
  auto check() -> bool;
  void build();
  auto read_record() -> bool;

  struct record_entry {
    uint32_t addr = 0;
    uint16_t length = 0;
    std::vector<uint8_t> data;
  };

  std::vector<record_entry> records;
  std::ifstream ips_file;
  size_t min_size = 0;
};
} // namespace nes::utility
