#pragma once

#include <fstream>

namespace nes {
class Debugger final {
public:
  Debugger() = default;

  void cpu_log();

private:
  std::ofstream nestest_log;
};
} // namespace nes
