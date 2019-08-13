#include "timer.h"

namespace lib {
float timer::elapsed_time()
{
  const auto end = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<float> elapsed = end - start;

  return elapsed.count();
}

void timer::restart()
{
  start = std::chrono::high_resolution_clock::now();
}
}  // namespace lib
