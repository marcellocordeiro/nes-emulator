#pragma once

namespace nes::types::cartridge {
enum mirroring_type
{
  Unset = -1,
  Horizontal,
  Vertical,
  One_Screen_Low,
  One_Screen_High,
  Four_Screen
};
}  // namespace nes::types::cartridge
