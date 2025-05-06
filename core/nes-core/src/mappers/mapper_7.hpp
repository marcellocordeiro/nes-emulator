#pragma once

#include "../base_mapper.hpp"

namespace nes {
class Mapper7 final : public BaseMapper {
public:
  void reset() override;

  void write(u16, u8) override;

  void save(std::ofstream& out) const override;
  void load(std::ifstream& in) override;

private:
  void apply();

  i32 mode = 0;
};
} // namespace nes
