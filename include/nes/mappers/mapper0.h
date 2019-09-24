#pragma once

#include "nes/base_mapper.h"

namespace nes {
class mapper0 : public base_mapper {
public:
  mapper0(cartridge&);

  void reset() override;
};
}  // namespace nes
