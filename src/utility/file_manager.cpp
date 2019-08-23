#include "utility/file_manager.h"

#include "log.h"
#include "system_utils.h"

namespace nes::util {
file_manager::file_manager()
{
  try {
    set_app_path(lib::get_app_path());
  } catch (const std::exception& e) {
    lib::message_box(e.what());
    std::exit(1);
  }
}

void file_manager::set_app_path(const std::filesystem::path& value)
{
  app_path = std::filesystem::canonical(value);  // May throw
}

void file_manager::set_rom(const std::filesystem::path& value)
{
  rom      = std::filesystem::canonical(value);  // May throw
  prg_ram  = rom;
  patch    = rom;
  snapshot = rom;

  prg_ram.replace_extension(".srm");
  patch.replace_extension(".ips");
  snapshot.replace_extension(".state");

  if (std::filesystem::exists(prg_ram)) {
    LOG(Info, "Found PRG-RAM file");
  }

  if (std::filesystem::exists(patch)) {
    LOG(Info, "Found IPS patch file");
  }

  if (std::filesystem::exists(snapshot)) {
    LOG(Info, "Found snapshot file");
  }
}

void file_manager::set_palette(const std::filesystem::path& value)
{
  palette = std::filesystem::canonical(value);  // May throw
}

std::filesystem::path file_manager::get_app_path() const
{
  return app_path;
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

std::filesystem::path file_manager::get_snapshot() const
{
  return snapshot;
}

bool file_manager::has_patch() const
{
  return std::filesystem::exists(patch);
}

bool file_manager::has_prg_ram() const
{
  return std::filesystem::exists(prg_ram);
}

bool file_manager::has_snapshot() const
{
  return std::filesystem::exists(snapshot);
}
}  // namespace nes::util
