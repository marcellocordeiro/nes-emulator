#pragma once

#include <fstream>

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

  void dump_snapshot(std::ofstream& out, bool value)
  {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void dump_snapshot(std::ofstream& out, int value)
  {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void dump_snapshot(std::ofstream& out, uint8_t value)
  {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void dump_snapshot(std::ofstream& out, uint16_t value)
  {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void dump_snapshot(std::ofstream& out, size_t value)
  {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  template <typename... Args>
  void get_snapshot(std::ifstream& in, Args&&... args)
  {
    (get_snapshot(in, std::forward<Args>(args)), ...);
  }

  void get_snapshot(std::ifstream& in, bool& value)
  {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void get_snapshot(std::ifstream& in, int& value)
  {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void get_snapshot(std::ifstream& in, uint8_t& value)
  {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void get_snapshot(std::ifstream& in, uint16_t& value)
  {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }

  void get_snapshot(std::ifstream& in, size_t& value)
  {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }
};
}  // namespace nes::util

/* template <typename... Args>
std::enable_if_t<
    ((std::is_same_v<std::decay_t<Args>, bool> ||
      std::is_same_v<std::decay_t<Args>, int>)&&...)>
test(Args&&... args)
{
  ((std::cout << std::forward<Args>(args) << " "), ...);
}

int foo()
{
  std::cout << std::boolalpha;
  bool a = false;
  int  b = 1;
  test(1, 2, 3, 4, true, false, false, a, b);
} */