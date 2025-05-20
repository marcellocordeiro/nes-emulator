#pragma once

#include "../base_mapper.hpp"

namespace nes {
class Mapper4 final: public BaseMapper {
public:
  void reset() override;

  void write(u16 addr, u8 value) override;

  void increment_scanline_counter() override;

private:
  void apply();

  std::array<u8, 8> regs = {};
  u8 reg_8000 = 0;

  bool irq_enabled = false;
  u8 irq_period = 0;
  u8 irq_counter = 0;
};
} // namespace nes
