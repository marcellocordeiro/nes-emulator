#pragma once

#include <fstream>

// #include <nes/utility/file_manager.h>

namespace nes {
class debugger {
public:
  debugger() = default;

  debugger(const debugger&) = delete;
  debugger(debugger&&)      = delete;
  debugger& operator=(const debugger&) = delete;
  debugger& operator=(debugger&&) = delete;

  void cpu_log();

private:
  std::ofstream
      nestest_log;  //{util::fmngr.get_app_path() / "nestest_out.log"};
};
}  // namespace nes
