#pragma once

#include "../base_mapper.hpp"

namespace nes {
class Mapper0 final: public BaseMapper {
public:
  void reset() override;
};
} // namespace nes
