#pragma once

#include <iostream>

#define LOG(level)                            \
  if ((level) <= lib::log::get().get_level()) \
  lib::log::get().get_stream()                \
      << '\n'                                 \
      << '[' << __FILE__ << ":" << std::dec << __LINE__ << "] "

namespace lib {
class log {
public:
  enum log_level { None, Error, Info, Verbose };

  void set_stream(std::ostream&);
  void set_level(log_level);

  std::ostream& get_stream();
  log_level     get_level() const;

  static log& get();

private:
  log_level     level  = Info;
  std::ostream* stream = &std::cout;
};
}  // namespace lib
