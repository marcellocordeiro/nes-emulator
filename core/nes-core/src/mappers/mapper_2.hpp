#pragma once

#include "../base_mapper.hpp"

namespace nes {
class Mapper2 final: public BaseMapper {
public:
  void reset() override;

  void write(u16 addr, u8 value) override;

private:
  void apply();

  u8 mode = 0;
};
} // namespace nes
