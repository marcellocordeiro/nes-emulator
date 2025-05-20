#pragma once

#include <memory>
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

  void load();

  [[nodiscard]] auto prg_read(u16 addr) const -> u8;
  [[nodiscard]] auto chr_read(u16 addr) const -> u8;

  void prg_write(u16 addr, u8 value);
  void chr_write(u16 addr, u8 value);

  void scanline_counter() const;

  void dump_prg_ram() const;

  std::shared_ptr<bool> irq_conn;

private:
  Cartridge() = default;

  i32 mapper_num = -1;
  usize prg_size = 0;
  usize chr_size = 0;
  bool has_chr_ram = false;
  usize prg_ram_size = 0;

  std::unique_ptr<BaseMapper> mapper;

  std::vector<u8> rom;

  std::span<u8> prg;
  std::span<u8> chr;

  std::vector<u8> prg_ram;
  std::vector<u8> chr_ram;
};
} // namespace nes
