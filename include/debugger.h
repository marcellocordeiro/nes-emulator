#pragma once

#include <fstream>

#include "types/forward_decl.h"
#include "utility/file_manager.h"

namespace nes {
class debugger {
public:
  debugger(nes::emulator&);

  void cpu_log();

private:
  std::ofstream  nestest_log{util::fmngr.get_app_path() / "nestest_out.log"};
  nes::emulator& emulator;

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
