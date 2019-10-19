#include "FileManager.h"

#include <fstream>

#include "Patch.h"
#include <system_utils.h>

#include <spdlog/spdlog.h>

namespace nes::util {
file_manager& file_manager::get()
{
  static file_manager instance;
  return instance;
}

void file_manager::setup()
{
  set_app_path(lib::get_app_path());
  set_palette(app_path / "palette.pal");
}

void file_manager::set_app_path(const std::filesystem::path& value)
{
  app_path = std::filesystem::canonical(value);  // May throw
}

void file_manager::set_rom(const std::filesystem::path& value)
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

std::vector<std::uint8_t> file_manager::get_rom()
{
  if (!std::filesystem::exists(rom_path)) {
    throw std::runtime_error("The ROM path needs to be set first");
  }

  std::ifstream rom_stream(rom_path, std::ios::binary);

  if (!rom_stream) {
    throw std::runtime_error("Couldn't open the ROM");
  }

  std::vector rom(std::filesystem::file_size(rom_path), uint8_t{});
  rom_stream.read(reinterpret_cast<char*>(rom.data()), rom.size());

  if (has_patch()) {
    rom = ips_patch(patch_path).patch(rom);
  }

  return rom;
}

std::vector<std::uint8_t> file_manager::get_prg_ram(std::size_t size)
{
  std::vector prg_ram(size, uint8_t{});

  if (has_prg_ram()) {
    std::ifstream prg_ram_file(prg_ram_path, std::ios::binary);
    prg_ram_file.read(reinterpret_cast<char*>(prg_ram.data()), prg_ram.size());
  }

  return prg_ram;
}

void file_manager::set_palette(const std::filesystem::path& value)
{
  palette_path = std::filesystem::canonical(value);  // May throw
}

std::filesystem::path file_manager::get_app_path() const { return app_path; }

std::filesystem::path file_manager::get_rom_path() const { return rom_path; }

std::filesystem::path file_manager::get_patch_path() const
{
  return patch_path;
}

std::filesystem::path file_manager::get_prg_ram_path() const
{
  return prg_ram_path;
}

std::filesystem::path file_manager::get_palette_path() const
{
  return palette_path;
}

std::filesystem::path file_manager::get_snapshot_path() const
{
  return snapshot_path;
}

bool file_manager::has_patch() const
{
  return std::filesystem::exists(patch_path);
}

bool file_manager::has_prg_ram() const
{
  return std::filesystem::exists(prg_ram_path);
}

bool file_manager::has_snapshot() const
{
  return std::filesystem::exists(snapshot_path);
}
}  // namespace nes::util
