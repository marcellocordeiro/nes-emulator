#pragma once

#include "../BaseMapper.h"

namespace nes {
class mapper2 : public base_mapper {
public:
  mapper2() = default;

  void reset() override;

  void prg_write(uint16_t, uint8_t) override;

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

private:
  void apply();

  int mode = 0;
};
}  // namespace nes