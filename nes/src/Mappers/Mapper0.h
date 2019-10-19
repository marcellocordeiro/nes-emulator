#pragma once

#include "../BaseMapper.h"

namespace nes {
class mapper0 : public base_mapper {
public:
  mapper0() = default;

  void reset() override;
};
}  // namespace nes
