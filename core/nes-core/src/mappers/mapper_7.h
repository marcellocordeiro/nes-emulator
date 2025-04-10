#pragma once

#include "../base_mapper.h"

namespace nes {
class Mapper7 final : public BaseMapper {
public:
  void reset() override;

  void write(uint16_t, uint8_t) override;

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  void apply();

  int mode = 0;
};
} // namespace nes
