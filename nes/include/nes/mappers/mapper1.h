#pragma once

#include "nes/base_mapper.h"

namespace nes {
class mapper1 : public base_mapper {
public:
  mapper1(cartridge&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  void apply();

  int     write_delay = 5;
  uint8_t shift_reg   = 0;

  uint8_t control    = 0x0C;
  uint8_t chr_bank_0 = 0;
  uint8_t chr_bank_1 = 0;
  uint8_t prg_bank   = 0;
};
}  // namespace nes