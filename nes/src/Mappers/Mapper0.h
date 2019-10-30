#pragma once

#include "../BaseMapper.h"

namespace nes {
class Mapper0 final : public BaseMapper {
public:
  Mapper0() = default;

  void reset() override;
};
}  // namespace nes
