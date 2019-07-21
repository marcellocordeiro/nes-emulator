#pragma once

#include "mapper.h"

namespace nes {
class mapper0 : public mapper {
public:
  mapper0(nes::cartridge&);

  void reset() override;
};
}  // namespace nes
