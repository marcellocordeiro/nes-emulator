#include "utility/file_manager.h"

#include "log.h"

namespace nes::util {
void file_manager::set_rom(const std::filesystem::path& value)
{
  rom     = std::filesystem::canonical(value);  // May throw
  prg_ram = std::filesystem::path(rom).replace_extension(".srm");
  patch   = std::filesystem::path(rom).replace_extension(".ips");

  if (std::filesystem::exists(prg_ram)) {
    LOG(lib::log::Info) << "Found PRG-RAM file";
  }

  if (std::filesystem::exists(patch)) {
    LOG(lib::log::Info) << "Found IPS patch file";
  }
}

void file_manager::set_palette(const std::filesystem::path& value)
{
  palette = std::filesystem::canonical(value);  // May throw
}

std::filesystem::path file_manager::get_rom() const
{
  return rom;
}

std::filesystem::path file_manager::get_patch() const
{
  return patch;
}

std::filesystem::path file_manager::get_prg_ram() const
{
  return prg_ram;
}

std::filesystem::path file_manager::get_palette() const
{
  return palette;
}

bool file_manager::has_patch() const
{
  return std::filesystem::exists(patch);
}

bool file_manager::has_prg_ram() const
{
  return std::filesystem::exists(prg_ram);
}
}  // namespace nes::util
