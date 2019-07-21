#pragma once

#include "mapper.h"

namespace nes {
class mapper1 : public mapper {
public:
  mapper1(nes::cartridge&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;

private:
  void apply();

  int     write_counter = 0;
  uint8_t shift_reg     = 0;

  uint8_t control    = 0x0C;
  uint8_t chr_bank_0 = 0;
  uint8_t chr_bank_1 = 0;
  uint8_t prg_bank   = 0;
};
}  // namespace nes
