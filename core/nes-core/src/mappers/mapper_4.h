#pragma once

#include "../base_mapper.h"

namespace nes {
class Mapper4 final : public BaseMapper {
public:
  void reset() override;

  void write(uint16_t, uint8_t) override;

  void scanline_counter() override;

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  void apply();

  std::array<uint8_t, 8> regs = {};
  uint8_t reg_8000 = 0;

  bool horizontal_mirroring = false;

  bool irq_enabled = false;
  uint8_t irq_period = 0;
  uint8_t irq_counter = 0;
};
}  // namespace nes
