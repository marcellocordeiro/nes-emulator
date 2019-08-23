#pragma once

#include <iostream>

#include <fmt/format.h>

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
  log() = default;

  int           level  = Info;
  std::ostream* stream = &std::cout;
};
}  // namespace lib

#define LOG(level, ...)                                                       \
  if ((level) <= lib::log::get().get_level())                                 \
  lib::log::get().get_stream() << fmt::format("[{}:{}] ", __FILE__, __LINE__) \
                               << fmt::format(__VA_ARGS__) << '\n'
