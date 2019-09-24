#pragma once

#include <fstream>

#include <nes/utility/file_manager.h>

namespace nes {
class emulator;

class debugger {
public:
  debugger(emulator&);
  ~debugger() = default;

  debugger(const debugger&) = delete;
  debugger(debugger&&)      = delete;
  debugger& operator=(const debugger&) = delete;
  debugger& operator=(debugger&&) = delete;

  void cpu_log();

private:
  emulator& emu;

  std::ofstream nestest_log{util::fmngr.get_app_path() / "nestest_out.log"};
};
}  // namespace nes
