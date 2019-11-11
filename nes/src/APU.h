#pragma once

namespace nes {
class APU final {
public:
  APU(const APU&) = delete;
  APU(APU&&)      = delete;
  APU& operator=(const APU&) = delete;
  APU& operator=(APU&&) = delete;

  static APU& get();

private:
  APU() = default;
};
}  // namespace nes
