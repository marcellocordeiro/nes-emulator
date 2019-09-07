#pragma once

#include <memory>

#include "common.h"
#include "types/cartridge.h"

namespace nes {
class emulator;
class base_mapper;

class cartridge {
public:
  cartridge(emulator&);
  ~cartridge();  // Has to be defined in the cpp

  base_mapper* get_mapper();

  void load();

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

  void dump_prg_ram() const;

  void set_mirroring(int);
  void set_cpu_irq(bool);

private:
  emulator& emu;

  cartridge_info info{};

  std::unique_ptr<base_mapper> mapper;
};
}  // namespace nes
