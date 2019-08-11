#pragma once

#include <iostream>

#define LOG(level, expr)                      \
  if ((level) <= lib::log::get().get_level()) \
    lib::log::get().get_stream()              \
        << '[' << __FILE__ << ":" << __LINE__ << "] " << expr << '\n';

enum log_level { None, Error, Info, Verbose };

namespace lib {
class log {
public:
  void set_stream(std::ostream&);
  void set_level(int);

  std::ostream& get_stream();
  int           get_level() const;

  static log& get();

private:
  int           level  = Info;
  std::ostream* stream = &std::cout;
};
}  // namespace lib
