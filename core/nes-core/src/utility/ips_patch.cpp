#include "ips_patch.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "lib/common.hpp"

namespace nes::utility {
IpsPatch::IpsPatch(const std::filesystem::path& path) : ips_file(path, std::ios::binary) {
  build();
}

void IpsPatch::build() {
  if (!check()) {
    throw std::runtime_error("Invalid IPS patch");
  }

  while (read_record()) {
    min_size = std::max(min_size, records.back().addr + static_cast<usize>(records.back().length));
  }
}

auto IpsPatch::patch(const std::vector<u8>& rom) -> std::vector<u8> {
  std::vector<u8> output(std::max(min_size, rom.size()));

  std::ranges::copy(rom, output.begin());

  for (const auto& entry : records) {
    std::ranges::copy(entry.data, output.begin() + entry.addr);
  }

  //
  // Truncate (extension)
  //

  std::array<u8, 3> buffer = {};
  ips_file.read(reinterpret_cast<char*>(buffer.data()), 3 * sizeof(u8));

  // If the stream is still good, there is a 3-byte truncate offset after EOF
  if (ips_file) {
    const usize truncate_offset =
      static_cast<usize>(buffer[0] << 16) | static_cast<usize>(buffer[1] << 8) | buffer[2];

    if (output.size() > truncate_offset) {
      output.resize(truncate_offset);
    }
  }

  return output;
}

auto IpsPatch::check() -> bool {
  std::string header;
  header.resize(5);
  ips_file.read(header.data(), 5);

  return header == "PATCH";
}

auto IpsPatch::read_record() -> bool {
  constexpr auto magic_eof = static_cast<u32>((('E') << 16) | ('O' << 8) | ('F'));

  RecordEntry record;

  std::array<u8, 3> buffer = {};

  ips_file.read(reinterpret_cast<char*>(buffer.data()), 3 * sizeof(u8));
  record.addr = static_cast<u32>(buffer[0] << 16) | static_cast<u32>(buffer[1] << 8) | buffer[2];

  if (record.addr == magic_eof) {
    return false;
  }

  ips_file.read(reinterpret_cast<char*>(buffer.data()), 2);
  record.length = static_cast<u16>(buffer[0] << 8) | buffer[1];

  if (record.length > 0u) {
    record.data.resize(record.length);
    ips_file.read(reinterpret_cast<char*>(record.data.data()), record.length);
  } else { // RLE
    ips_file.read(reinterpret_cast<char*>(buffer.data()), 3);
    record.length = static_cast<u16>(buffer[0] << 8) | buffer[1];
    const u8 value = buffer[2];

    record.data.resize(record.length);
    std::fill_n(record.data.begin(), record.length, value);
  }

  records.push_back(std::move(record));

  return true;
}
} // namespace nes::utility
