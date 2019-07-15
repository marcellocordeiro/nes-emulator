#pragma once

#include <memory>

#include "bus.h"
#include "common.h"
#include "mapper.h"
#include "types/cartridge.h"

namespace nes {
class cartridge {
public:
  void set_bus(nes::bus&);

  void load(const char*);

  void dump_prg_ram() const;

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

private:
  nes::bus* bus = nullptr;

  nes::cartridge_info info{};

  std::unique_ptr<nes::mapper> mapper = nullptr;
};
}  // namespace nes
