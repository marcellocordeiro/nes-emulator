#pragma once

#include <memory>

#include <common.h>

#include "BaseMapper.h"
#include "Types/Cartridge_Types.h"

namespace nes {
class Cartridge final {
public:
  Cartridge(const Cartridge&) = delete;
  Cartridge(Cartridge&&)      = delete;
  Cartridge& operator=(const Cartridge&) = delete;
  Cartridge& operator=(Cartridge&&) = delete;

  static Cartridge& get();

  BaseMapper* get_mapper();

  void load();

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

  void dump_prg_ram() const;

private:
  Cartridge() = default;

  std::unique_ptr<BaseMapper> mapper;
};
}  // namespace nes
