#pragma once

#include "mapper.h"

namespace nes {
class mapper4 : public mapper {
public:
  mapper4(nes::cartridge&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;

  void scanline_counter() override;

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  void apply();

  std::array<uint8_t, 8> regs;
  uint8_t                reg_8000;

  bool horizontal_mirroring;

  uint8_t irq_period;
  uint8_t irq_counter;
  bool    irq_enabled;
};
}  // namespace nes
