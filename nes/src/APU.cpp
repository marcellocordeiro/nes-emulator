#include "APU.h"

namespace nes {
APU& APU::get()
{
  static APU instance;
  return instance;
}
}  // namespace nes
