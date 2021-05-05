#include "FileManager.h"

#include <fstream>

#include <spdlog/spdlog.h>

#include "Patch.h"

namespace nes::Utility {
FileManager& FileManager::get()
{
  static FileManager instance;
  return instance;
}

void FileManager::set_app_path(const std::filesystem::path& value)
{
  app_path = std::filesystem::canonical(value);  // May throw
  set_palette(app_path / "palette.pal");
}

void FileManager::set_rom(const std::filesystem::path& value)
{
  rom_path      = std::filesystem::canonical(value);  // May throw
  prg_ram_path  = rom_path;
  patch_path    = rom_path;
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

void FileManager::set_palette(const std::filesystem::path& value)
{
  palette_path = std::filesystem::canonical(value);  // May throw
}

std::vector<std::uint8_t> FileManager::get_rom()
{
  if (!std::filesystem::exists(rom_path)) {
    throw std::invalid_argument("The ROM path needs to be set first");
  }

  std::ifstream rom_stream(rom_path, std::ios::binary);

  std::vector rom(std::filesystem::file_size(rom_path), uint8_t{});
  rom_stream.read(reinterpret_cast<char*>(rom.data()), rom.size());

  if (has_patch()) {
    rom = ips_patch(patch_path).patch(rom);
  }

  return rom;
}

std::vector<std::uint8_t> FileManager::get_prg_ram()
{
  std::vector prg_ram(std::filesystem::file_size(prg_ram_path), uint8_t{});

  if (has_prg_ram()) {
    std::ifstream prg_ram_file(prg_ram_path, std::ios::binary);
    prg_ram_file.read(reinterpret_cast<char*>(prg_ram.data()), prg_ram.size());
  }

  return prg_ram;
}

std::vector<std::uint8_t> FileManager::get_palette()
{
  std::ifstream file(palette_path, std::ios::binary);

  std::vector<uint8_t> palette(std::filesystem::file_size(palette_path), uint8_t{});
  file.read(reinterpret_cast<char*>(palette.data()), palette.size());

  return palette;
}

void FileManager::save_prg_ram(const std::vector<std::uint8_t>& vec)
{
  std::ofstream prg_ram_file(prg_ram_path, std::ios::binary);
  prg_ram_file.write(reinterpret_cast<const char*>(vec.data()), vec.size());
}

std::filesystem::path FileManager::get_app_path() const { return app_path; }

std::filesystem::path FileManager::get_rom_path() const { return rom_path; }

std::filesystem::path FileManager::get_patch_path() const { return patch_path; }

std::filesystem::path FileManager::get_prg_ram_path() const { return prg_ram_path; }

std::filesystem::path FileManager::get_palette_path() const { return palette_path; }

std::filesystem::path FileManager::get_snapshot_path() const { return snapshot_path; }

bool FileManager::has_patch() const { return std::filesystem::exists(patch_path); }

bool FileManager::has_prg_ram() const { return std::filesystem::exists(prg_ram_path); }

bool FileManager::has_snapshot() const { return std::filesystem::exists(snapshot_path); }
}  // namespace nes::Utility
