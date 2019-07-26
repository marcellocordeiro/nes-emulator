#pragma once

#include <memory>

#include "common.h"
#include "mapper.h"
#include "types/cartridge.h"
#include "types/forward_decl.h"

namespace nes {
class cartridge {
public:
  cartridge(nes::cpu&, nes::ppu&);

  void load(const char*);

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  void prg_write(uint16_t, uint8_t);
  void chr_write(uint16_t, uint8_t);

  void scanline_counter();

  void dump_prg_ram() const;

  void set_mirroring(int);
  void set_cpu_irq(bool);

private:
  nes::cpu& cpu;
  nes::ppu& ppu;

  nes::cartridge_info info{};

  std::unique_ptr<nes::mapper> mapper = nullptr;
};
}  // namespace nes
