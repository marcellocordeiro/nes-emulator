#pragma once

#include "mapper.h"

namespace nes {
class mapper0 : public mapper {
public:
  mapper0(
      const nes::cartridge_info&,
      std::vector<uint8_t>&&,
      std::vector<uint8_t>&&);

  void reset() override;
};
}  // namespace nes
