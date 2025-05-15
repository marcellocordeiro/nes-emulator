#pragma once

#include "../base_mapper.hpp"

namespace nes {
class Mapper1 final: public BaseMapper {
public:
  void reset() override;

  void write(u16, u8) override;

  void save(std::ofstream& out) const override;
  void load(std::ifstream& in) override;

private:
  void apply();

  i32 write_delay = 5;
  u8 shift_reg = 0;

  u8 control = 0x0C;
  u8 chr_bank_0 = 0;
  u8 chr_bank_1 = 0;
  u8 prg_bank = 0;
};
} // namespace nes
