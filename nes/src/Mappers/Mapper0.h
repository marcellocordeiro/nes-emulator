#pragma once

#include "../BaseMapper.h"

namespace nes {
class Mapper0 final : public BaseMapper {
public:
  void reset() override;
};
}  // namespace nes
