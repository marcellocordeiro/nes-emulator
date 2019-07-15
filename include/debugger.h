#pragma once

#if 0

#include <fstream>

#include "cpu.h"
#include "types.h"

namespace nes {
class debugger {
public:
  debugger(const nes::cpu& cpu_in) : cpu(cpu_in) {}

  void nestest();

private:
  std::ofstream   nestest_log{"../roms/nestest_out.log"};
  const nes::cpu& cpu;

  enum addr_mode2 {
    impl,
    acc,
    imm,
    zp,
    zpx,
    zpy,
    rel,
    ab,
    abx,
    abx_,
    aby,
    aby_,
    ind,
    indx,
    indy,
    indy_,
    inv
  };
};
}  // namespace nes
#endif
