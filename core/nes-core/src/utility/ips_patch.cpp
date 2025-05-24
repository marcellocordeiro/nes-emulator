#include "ips_patch.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "lib/common.hpp"

namespace nes::utility {
IpsPatch::IpsPatch(const std::vector<u8>& patch_file) {
  build(patch_file);
}

void IpsPatch::build(const std::vector<u8>& patch_file) {
  auto iterator = patch_file.cbegin();

  if (!check(iterator)) {
    throw std::runtime_error("Invalid IPS patch");
  }

  while (read_record(iterator)) {
    min_size = std::max(min_size, records.back().addr + records.back().data.size());
  }

  if (iterator != patch_file.cend()) {
    truncate_size = take_from_iterator<usize>(iterator, 3);
  }
}

auto IpsPatch::patch(const std::vector<u8>& rom) -> std::vector<u8> {
  std::vector<u8> output(std::max(min_size, rom.size()));

  std::ranges::copy(rom, output.begin());

  for (const auto& [addr, data] : records) {
    std::ranges::copy(data, output.begin() + addr);
  }

  //
  // Truncate (extension)
  //

  if (truncate_size) {
    if (output.size() > *truncate_size) {
      output.resize(*truncate_size);
    }
  }

  return output;
}

auto IpsPatch::check(std::vector<u8>::const_iterator& iterator) -> bool {
  const auto header = std::string(iterator, iterator + 5);
  std::advance(iterator, 5);

  return header == "PATCH";
}

auto IpsPatch::read_record(std::vector<u8>::const_iterator& iterator) -> bool {
  static constexpr auto EOF_MARKER = static_cast<u32>((('E') << 16) | ('O' << 8) | ('F'));

  const auto addr = take_from_iterator<u32>(iterator, 3);

  if (addr == EOF_MARKER) {
    return false;
  }

  const auto length = take_from_iterator<u16>(iterator, 2);

  std::vector<u8> data;

  if (length > 0u) {
    data.assign(iterator, iterator + length);
    std::advance(iterator, length);
  } else {
    // RLE
    const auto rle_length = take_from_iterator<u16>(iterator, 2);
    const auto rle_value = take_from_iterator<u8>(iterator, 1);

    data.assign(rle_length, rle_value);
  }

  records.emplace_back(addr, std::move(data));

  return true;
}

template <typename T>
auto IpsPatch::take_from_iterator(std::vector<u8>::const_iterator& iterator, const auto count) -> T {
  const auto range = std::ranges::subrange(iterator, iterator + count);
  const auto result = std::ranges::fold_left(range, T{0}, [](const T acc, const u8 value) {
    return static_cast<T>((acc << 8) | static_cast<T>(value));
  });
  std::advance(iterator, count);

  return result;
}
} // namespace nes::utility
