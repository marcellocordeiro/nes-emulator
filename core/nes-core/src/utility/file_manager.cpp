#include "file_manager.hpp"

#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <vector>

#include <spdlog/spdlog.h>

#include "ips_patch.hpp"
#include "lib/common.hpp"
#include "lib/files.hpp"

namespace nes::utility {
auto FileManager::get() -> FileManager& {
  static FileManager instance;
  return instance;
}

void FileManager::set_app_path(const std::filesystem::path& value) {
  app_path = std::filesystem::canonical(value); // May throw
  set_palette(app_path / "palette.pal");
}

void FileManager::set_rom(const std::filesystem::path& value) {
  rom_path = std::filesystem::canonical(value); // May throw
  prg_ram_path = rom_path;
  patch_path = rom_path;
  snapshot_path = rom_path;

  prg_ram_path.replace_extension(".srm");
  patch_path.replace_extension(".ips");
  snapshot_path.replace_extension(".state");

  if (std::filesystem::exists(prg_ram_path)) {
    spdlog::info("Found PRG-RAM file");
  }

  if (std::filesystem::exists(patch_path)) {
    spdlog::info("Found IPS patch file");
  }

  if (std::filesystem::exists(snapshot_path)) {
    spdlog::info("Found snapshot file");
  }
}

void FileManager::set_palette(const std::filesystem::path& value) {
  palette_path = std::filesystem::canonical(value); // May throw
}

auto FileManager::get_rom() const -> std::vector<u8> {
  if (!std::filesystem::exists(rom_path)) {
    throw std::invalid_argument("The ROM path needs to be set first");
  }

  auto rom = lib::read_binary_file(rom_path);

  if (has_patch()) {
    rom = IpsPatch(patch_path).patch(rom);
  }

  return rom;
}

auto FileManager::get_prg_ram() const -> std::vector<u8> {
  if (!has_prg_ram()) {
    return {};
  }

  return lib::read_binary_file(prg_ram_path);
}

auto FileManager::get_palette() const -> std::vector<u8> {
  return lib::read_binary_file(palette_path);
}

void FileManager::save_prg_ram(const std::vector<u8>& vec) const {
  std::ofstream prg_ram_file(prg_ram_path, std::ios::binary);
  prg_ram_file.write(
    reinterpret_cast<const char*>(vec.data()),
    static_cast<std::streamsize>(vec.size())
  );
}

auto FileManager::get_app_path() const -> std::filesystem::path {
  return app_path;
}

auto FileManager::get_rom_path() const -> std::filesystem::path {
  return rom_path;
}

auto FileManager::get_patch_path() const -> std::filesystem::path {
  return patch_path;
}

auto FileManager::get_prg_ram_path() const -> std::filesystem::path {
  return prg_ram_path;
}

auto FileManager::get_palette_path() const -> std::filesystem::path {
  return palette_path;
}

auto FileManager::get_snapshot_path() const -> std::filesystem::path {
  return snapshot_path;
}

auto FileManager::has_patch() const -> bool {
  return std::filesystem::exists(patch_path);
}

auto FileManager::has_prg_ram() const -> bool {
  return std::filesystem::exists(prg_ram_path);
}

auto FileManager::has_snapshot() const -> bool {
  return std::filesystem::exists(snapshot_path);
}
} // namespace nes::utility
