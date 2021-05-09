#pragma once

#include <fstream>

// #include <nes/utility/file_manager.h>

namespace nes {
class Debugger final {
public:
  Debugger() = default;

  Debugger(const Debugger&) = delete;
  Debugger(Debugger&&)      = delete;
  auto operator=(const Debugger&) -> Debugger& = delete;
  auto operator=(Debugger&&) -> Debugger& = delete;

  void cpu_log();

private:
  std::ofstream nestest_log;  //{util::fmngr.get_app_path() / "nestest_out.log"};
};
}  // namespace nes
