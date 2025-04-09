#pragma once

namespace nes {
template <typename T>
class Connection {
public:
  void set(T value) {
    status = value;
  }
  auto get() const -> T {
    return status;
  }

private:
  T status;
};
}  // namespace nes
