#pragma once

#include <fstream>

namespace nes {
class Debugger final {
public:
  Debugger() = default;

  Debugger(const Debugger&) = delete;
  Debugger(Debugger&&) = delete;
  auto operator=(const Debugger&) -> Debugger& = delete;
  auto operator=(Debugger&&) -> Debugger& = delete;

  void cpu_log();

private:
  std::ofstream nestest_log;
};
} // namespace nes
