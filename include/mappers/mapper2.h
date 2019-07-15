#pragma once

#include "mapper.h"

namespace nes {
class mapper2 : public mapper {
public:
  mapper2(
      const nes::cartridge_info&,
      std::vector<uint8_t>&&,
      std::vector<uint8_t>&&);

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;
};
}  // namespace nes
