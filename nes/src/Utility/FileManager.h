#pragma once

#include <filesystem>
#include <vector>

namespace nes::Utility {
class FileManager final {
public:
  FileManager(const FileManager&) = delete;
  FileManager(FileManager&&)      = delete;
  FileManager& operator=(const FileManager&) = delete;
  FileManager& operator=(FileManager&&) = delete;

  static FileManager& get();

  void set_app_path(const std::filesystem::path&);
  void set_rom(const std::filesystem::path&);
  void set_palette(const std::filesystem::path&);

  std::vector<std::uint8_t> get_rom();
  std::vector<std::uint8_t> get_prg_ram();
  std::vector<std::uint8_t> get_palette();

  void save_prg_ram(const std::vector<std::uint8_t>&);

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
  FileManager() = default;

  std::filesystem::path app_path;
  std::filesystem::path rom_path;
  std::filesystem::path patch_path;
  std::filesystem::path prg_ram_path;
  std::filesystem::path palette_path;
  std::filesystem::path snapshot_path;
};
}  // namespace nes::Utility
