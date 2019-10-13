#include "timer.h"

namespace lib {
std::chrono::nanoseconds timer::elapsed_time()
{
  return std::chrono::steady_clock::now() - start;
}

void timer::restart() { start = std::chrono::steady_clock::now(); }
}  // namespace lib
