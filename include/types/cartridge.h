#pragma once

#include "common.h"

namespace nes {
namespace ct {
constexpr uint16_t prg_bank_size = 0x4000;  // 16384;
constexpr uint16_t chr_bank_size = 0x2000;  // 8192;
}  // namespace ct

namespace mirroring {
enum mirroring {
  Unset = -1,
  Horizontal,
  Vertical,
  One_Screen_Low,
  One_Screen_High,
  Four_Screen
};
}

struct cartridge_info {
  const char* path;
  size_t      rom_size;
  size_t      mapper_num;
  size_t      prg_size;
  size_t      chr_size;
  size_t      prg_ram_size;
  int         mirroring = mirroring::Unset;

  bool chr_ram = false;
};
}  // namespace nes
