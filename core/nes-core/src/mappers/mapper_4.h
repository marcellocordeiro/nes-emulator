#pragma once

#include "../base_mapper.h"

namespace nes {
class Mapper4 final : public BaseMapper {
public:
  void reset() override;

  void write(u16, u8) override;

  void scanline_counter() override;

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  void apply();

  std::array<u8, 8> regs = {};
  u8 reg_8000 = 0;

  bool horizontal_mirroring = false;

  bool irq_enabled = false;
  u8 irq_period = 0;
  u8 irq_counter = 0;
};
} // namespace nes
