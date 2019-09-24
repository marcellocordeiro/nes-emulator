#pragma once

#include <chrono>

namespace lib {
class timer {
public:
  std::chrono::nanoseconds elapsed_time();
  void                     restart();

private:
  std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();
};
}  // namespace lib
