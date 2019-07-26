#pragma once

#include <filesystem>

namespace nes::util {
class file_manager {
public:
  void set_rom(const std::filesystem::path&);
  void set_palette(const std::filesystem::path&);

  std::filesystem::path get_rom() const;
  std::filesystem::path get_patch() const;
  std::filesystem::path get_prg_ram() const;
  std::filesystem::path get_palette() const;

  bool has_prg_ram() const;
  bool has_patch() const;

private:
  std::filesystem::path rom;
  std::filesystem::path patch;
  std::filesystem::path prg_ram;
  std::filesystem::path palette;
};

inline file_manager fmngr;
}  // namespace nes::util
