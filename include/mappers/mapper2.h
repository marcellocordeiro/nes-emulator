#pragma once

#include "mapper.h"

namespace nes {
class mapper2 : public mapper {
public:
  mapper2(nes::cartridge&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;
};
}  // namespace nes
