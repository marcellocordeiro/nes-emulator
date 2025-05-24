#pragma once

#include <optional>
#include <vector>

#include "lib/common.hpp"

namespace nes::utility {
class IpsPatch final {
public:
  explicit IpsPatch(const std::vector<u8>& patch_file);

  [[nodiscard]] auto patch(const std::vector<u8>& rom) -> std::vector<u8>;

private:
  void build(const std::vector<u8>& patch_file);

  [[nodiscard]] static auto check(std::vector<u8>::const_iterator& iterator) -> bool;
  [[nodiscard]] auto read_record(std::vector<u8>::const_iterator& iterator) -> bool;

  template <typename T>
  auto take_from_iterator(std::vector<u8>::const_iterator& iterator, auto count) -> T;

  struct RecordEntry {
    u32 addr = 0;
    std::vector<u8> data;
  };

  std::vector<RecordEntry> records;

  usize min_size = 0;

  std::optional<usize> truncate_size;
};
} // namespace nes::utility
