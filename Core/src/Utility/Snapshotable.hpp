#pragma once

#include <array>
#include <fstream>
#include <type_traits>
#include <vector>

#include <common.h>

namespace nes::Utility {
class Snapshotable {
public:
  Snapshotable()          = default;
  virtual ~Snapshotable() = default;

  virtual void save(std::ofstream&) = 0;
  virtual void load(std::ifstream&) = 0;

protected:
  template <typename... Args> void dump_snapshot(std::ofstream& out, Args&&... args)
  {
    (dump(out, std::forward<Args>(args)), ...);
  }

  template <typename... Args> void get_snapshot(std::ifstream& in, Args&&... args)
  {
    (get(in, std::forward<Args>(args)), ...);
  }

private:
  //
  // dump specialisations
  //

  template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>> void dump(std::ofstream& out, T value)
  {
    static_assert(std::is_integral_v<T>);
    out.write(reinterpret_cast<char*>(&value), sizeof(T));
  }

  template <typename T, std::size_t size> void dump(std::ofstream& out, std::array<T, size>& arr)
  {
    for (auto value : arr) dump(out, value);
  }

  template <typename T> void dump(std::ofstream& out, std::vector<T>& vec)
  {
    dump(out, vec.size());
    for (auto value : vec) dump(out, value);
  }

  //
  // get specialisations
  //

  template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>> void get(std::ifstream& in, T& value)
  {
    static_assert(std::is_integral_v<T>);
    in.read(reinterpret_cast<char*>(&value), sizeof(T));
  }

  template <typename T, std::size_t size> void get(std::ifstream& in, std::array<T, size>& arr)
  {
    for (auto& ref : arr) get(in, ref);
  }

  template <typename T> void get(std::ifstream& in, std::vector<T>& vec)
  {
    {
      std::size_t size = 0;
      get(in, size);
      vec.resize(size);
    }

    for (auto& ref : vec) get(in, ref);
  }
};
}  // namespace nes::Utility
