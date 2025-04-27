#pragma once

#include <filesystem>
#include <vector>

#include <lib/common.h>

namespace nes::utility {
class FileManager final {
public:
  FileManager(const FileManager&) = delete;
  FileManager(FileManager&&) = delete;
  auto operator=(const FileManager&) -> FileManager& = delete;
  auto operator=(FileManager&&) -> FileManager& = delete;

  static auto get() -> FileManager&;

  void set_app_path(const std::filesystem::path&);
  void set_rom(const std::filesystem::path&);
  void set_palette(const std::filesystem::path&);

  auto get_rom() -> std::vector<u8>;
  auto get_prg_ram() -> std::vector<u8>;
  auto get_palette() -> std::vector<u8>;

  void save_prg_ram(const std::vector<u8>&);

  auto get_app_path() const -> std::filesystem::path;
  auto get_rom_path() const -> std::filesystem::path;
  auto get_patch_path() const -> std::filesystem::path;
  auto get_prg_ram_path() const -> std::filesystem::path;
  auto get_palette_path() const -> std::filesystem::path;
  auto get_snapshot_path() const -> std::filesystem::path;

  auto has_patch() const -> bool;
  auto has_prg_ram() const -> bool;
  auto has_snapshot() const -> bool;

private:
  FileManager() = default;

  std::filesystem::path app_path;
  std::filesystem::path rom_path;
  std::filesystem::path patch_path;
  std::filesystem::path prg_ram_path;
  std::filesystem::path palette_path;
  std::filesystem::path snapshot_path;
};
} // namespace nes::utility
