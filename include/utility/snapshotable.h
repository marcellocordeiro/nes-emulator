#pragma once

#include <fstream>
#include <type_traits>

#include "common.h"

namespace nes::util {
class snapshotable {
public:
  virtual void save(std::ofstream&) = 0;
  virtual void load(std::ifstream&) = 0;

protected:
  template <typename... Args>
  void dump_snapshot(std::ofstream& out, Args&&... args)
  {
    (dump_snapshot(out, std::forward<Args>(args)), ...);
  }

  template <typename T> void dump_snapshot(std::ofstream& out, T value)
  {
    static_assert(std::is_integral_v<T>);
    out.write(reinterpret_cast<char*>(&value), sizeof(T));
  }

  template <typename... Args>
  void get_snapshot(std::ifstream& in, Args&&... args)
  {
    (get_snapshot(in, std::forward<Args>(args)), ...);
  }

  template <typename T> void get_snapshot(std::ifstream& in, T& value)
  {
    static_assert(std::is_integral_v<T>);
    in.read(reinterpret_cast<char*>(&value), sizeof(T));
  }
};
}  // namespace nes::util
