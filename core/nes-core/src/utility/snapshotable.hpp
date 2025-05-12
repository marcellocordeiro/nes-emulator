#pragma once

#include <array>
#include <concepts>
#include <fstream>
#include <vector>

#include "lib/common.hpp"

namespace nes::utility {
class Snapshotable {
public:
  Snapshotable() = default;
  virtual ~Snapshotable() = default;

  Snapshotable(const Snapshotable&) = default;
  Snapshotable(Snapshotable&&) = default;
  auto operator=(const Snapshotable&) -> Snapshotable& = default;
  auto operator=(Snapshotable&&) -> Snapshotable& = default;

  virtual void save(std::ofstream& out) const = 0;
  virtual void load(std::ifstream& in) = 0;

protected:
  template <typename... Args>
  void dump_snapshot(std::ofstream& out, const Args&... args) const {
    (dump(out, args), ...);
  }

  template <typename... Args>
  void get_snapshot(std::ifstream& in, Args&&... args) {
    (get(in, std::forward<Args>(args)), ...);
  }

private:
  //
  // dump specialisations
  //

  static void dump(std::ofstream& out, std::integral auto value) {
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
  }

  template <typename T, std::size_t Size>
  void dump(std::ofstream& out, const std::array<T, Size>& arr) const {
    for (auto value : arr) {
      dump(out, value);
    }
  }

  template <typename T>
  void dump(std::ofstream& out, const std::vector<T>& vec) const {
    dump(out, vec.size());
    for (auto value : vec) {
      dump(out, value);
    }
  }

  //
  // get specialisations
  //

  static void get(std::ifstream& in, std::integral auto& value) {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
  }

  template <typename T, std::size_t Size>
  void get(std::ifstream& in, std::array<T, Size>& arr) {
    for (auto& ref : arr) {
      get(in, ref);
    }
  }

  template <typename T>
  void get(std::ifstream& in, std::vector<T>& vec) {
    {
      std::size_t size = 0;
      get(in, size);
      vec.resize(size);
    }

    for (auto& ref : vec) {
      get(in, ref);
    }
  }
};
} // namespace nes::utility
