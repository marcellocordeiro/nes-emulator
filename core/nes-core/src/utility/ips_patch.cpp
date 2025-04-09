#include "ips_patch.h"

#include <algorithm>
#include <string>

namespace nes::utility {
IpsPatch::IpsPatch(const std::filesystem::path& path) : ips_file(path, std::ios::binary) {
  build();
}

void IpsPatch::build() {
  if (!check()) {
    throw std::runtime_error("Invalid IPS patch");
  }

  while (read_record()) {
    min_size = std::max(min_size, records.back().addr + static_cast<size_t>(records.back().length));
  }
}

auto IpsPatch::patch(const std::vector<uint8_t>& rom) -> std::vector<uint8_t> {
  std::vector<uint8_t> output(std::max(min_size, rom.size()));

  std::copy(rom.begin(), rom.end(), output.begin());

  for (const auto& entry : records) {
    std::copy(entry.data.begin(), entry.data.end(), output.begin() + entry.addr);
  }

  //
  // Truncate (extension)
  //

  uint8_t buffer[] = {0, 0, 0};
  ips_file.read(reinterpret_cast<char*>(buffer), 3 * sizeof(uint8_t));

  // If the stream is still good, there is a 3-byte truncate offset after EOF
  if (ips_file) {
    size_t truncate_offset = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);

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
  constexpr auto magic_eof = (('E') << 16) | ('O' << 8) | ('F');

  record_entry record;

  uint8_t buffer[] = {0, 0, 0};
  ips_file.read(reinterpret_cast<char*>(buffer), 3 * sizeof(uint8_t));
  record.addr = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);

  if (record.addr == magic_eof) {
    return false;
  }

  ips_file.read(reinterpret_cast<char*>(buffer), 2);
  record.length = (buffer[0] << 8) | (buffer[1]);

  if (record.length > 0) {
    record.data.resize(record.length);
    ips_file.read(reinterpret_cast<char*>(record.data.data()), record.length);
  } else {  // RLE
    ips_file.read(reinterpret_cast<char*>(buffer), 3);
    record.length = (buffer[0] << 8) | (buffer[1]);
    uint8_t value = buffer[2];

    record.data.resize(record.length);
    std::fill(record.data.begin(), record.data.begin() + record.length, value);
  }

  records.push_back(std::move(record));

  return true;
}
}  // namespace nes::utility
