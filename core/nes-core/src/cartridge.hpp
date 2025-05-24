#pragma once

#include <memory>
#include <optional>
#include <span>
#include <vector>

#include "base_mapper.hpp"
#include "lib/common.hpp"

namespace nes {
class Cartridge final {
public:
  using MirroringType = BaseMapper::MirroringType;

  static auto get() -> Cartridge&;

  [[nodiscard]] auto get_mapper() const -> BaseMapper*;
  [[nodiscard]] auto get_mirroring() const -> MirroringType;

  void load(const std::vector<u8>& rom_file, const std::optional<std::vector<u8>>& prg_ram_file);

  [[nodiscard]] auto prg_read(u16 addr) const -> u8;
  [[nodiscard]] auto chr_read(u16 addr) const -> u8;

  void prg_write(u16 addr, u8 value);
  void chr_write(u16 addr, u8 value);

  void scanline_counter() const;

  [[nodiscard]] auto get_prg_ram() const -> std::vector<u8>;

  std::shared_ptr<bool> irq_conn;

private:
  Cartridge() = default;

  std::unique_ptr<BaseMapper> mapper;

  std::vector<u8> rom;

  std::span<u8> prg;
  std::span<u8> chr;

  std::vector<u8> prg_ram;
  std::vector<u8> chr_ram;
};
} // namespace nes
