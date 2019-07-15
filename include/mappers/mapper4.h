#pragma once

#include "mapper.h"

namespace nes {
class mapper4 : public mapper {
public:
  mapper4(
      const nes::cartridge_info&,
      std::vector<uint8_t>&&,
      std::vector<uint8_t>&&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;

  void scanline_counter() override;

private:
  uint8_t reg_8000;
  uint8_t regs[8];
  bool    horizontal_mirroring;

  uint8_t irq_period;
  uint8_t irq_counter;
  bool    irq_enabled;

  void apply();
};
}  // namespace nes
