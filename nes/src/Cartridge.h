#pragma once

#include <memory>

#include <common.h>

#include "BaseMapper.h"
#include "Types/Cartridge_Types.h"

namespace nes {
class cartridge {
public:
  cartridge(const cartridge&) = delete;
  cartridge(cartridge&&)      = delete;
  cartridge& operator=(const cartridge&) = delete;
  cartridge& operator=(cartridge&&) = delete;

  static cartridge& get();

  base_mapper* get_mapper();

  void load();

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

  void dump_prg_ram() const;

private:
  cartridge() = default;

  std::unique_ptr<base_mapper> mapper;
};
}  // namespace nes
