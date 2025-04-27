#pragma once

#include <memory>
#include <span>

#include <lib/common.h>

#include "base_mapper.h"

namespace nes {
class Cartridge final {
public:
  Cartridge(const Cartridge&) = delete;
  Cartridge(Cartridge&&) = delete;
  auto operator=(const Cartridge&) -> Cartridge& = delete;
  auto operator=(Cartridge&&) -> Cartridge& = delete;

  using MirroringType = BaseMapper::MirroringType;

  static auto get() -> Cartridge&;

  auto get_mapper() -> BaseMapper*;

  void load();

  auto prg_read(uint16_t) const -> uint8_t;
  auto chr_read(uint16_t) const -> uint8_t;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

  void dump_prg_ram() const;

  std::shared_ptr<bool> irq_conn;
  std::shared_ptr<MirroringType> mirroring_conn;

private:
  Cartridge() = default;

  int mapper_num = -1;
  size_t prg_size = 0;
  size_t chr_size = 0;
  bool has_chr_ram = false;
  size_t prg_ram_size = 0;

  MirroringType mirroring = MirroringType::Unknown;

  std::unique_ptr<BaseMapper> mapper;

  std::vector<uint8_t> rom;

  std::span<uint8_t> prg;
  std::span<uint8_t> chr;

  std::vector<uint8_t> prg_ram;
  std::vector<uint8_t> chr_ram;
};
} // namespace nes
