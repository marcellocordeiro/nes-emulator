#pragma once

#include <filesystem>
#include <vector>

namespace nes::util {
class file_manager {
public:
  file_manager(const file_manager&) = delete;
  file_manager(file_manager&&)      = delete;
  file_manager& operator=(const file_manager&) = delete;
  file_manager& operator=(file_manager&&) = delete;

  static file_manager& get();

  void setup();

  void set_app_path(const std::filesystem::path&);
  void set_rom(const std::filesystem::path&);
  void set_palette(const std::filesystem::path&);

  std::vector<std::uint8_t> get_rom();
  std::vector<std::uint8_t> get_prg_ram(std::size_t);

  std::filesystem::path get_app_path() const;
  std::filesystem::path get_rom_path() const;
  std::filesystem::path get_patch_path() const;
  std::filesystem::path get_prg_ram_path() const;
  std::filesystem::path get_palette_path() const;
  std::filesystem::path get_snapshot_path() const;

  bool has_patch() const;
  bool has_prg_ram() const;
  bool has_snapshot() const;

private:
  file_manager() = default;

  std::filesystem::path app_path;
  std::filesystem::path rom_path;
  std::filesystem::path patch_path;
  std::filesystem::path prg_ram_path;
  std::filesystem::path palette_path;
  std::filesystem::path snapshot_path;
};
}  // namespace nes::util