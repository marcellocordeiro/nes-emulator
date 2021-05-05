#pragma once

#include <fstream>

// #include <nes/utility/file_manager.h>

namespace nes {
class Debugger final {
public:
  Debugger() = default;

  Debugger(const Debugger&) = delete;
  Debugger(Debugger&&)      = delete;
  Debugger& operator=(const Debugger&) = delete;
  Debugger& operator=(Debugger&&) = delete;

  void cpu_log();

private:
  std::ofstream nestest_log;  //{util::fmngr.get_app_path() / "nestest_out.log"};
};
}  // namespace nes
