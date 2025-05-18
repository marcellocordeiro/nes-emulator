#include "controller.hpp"

#include "lib/common.hpp"

namespace nes {
auto Controller::get() -> Controller& {
  static Controller instance;
  return instance;
}

void Controller::update_state(const usize port, const u8 state) {
  controller_state[port] = state;
}

auto Controller::read(const usize port) -> u8 {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1); // 1 == A
  }

  const u8 status = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void Controller::write(const bool signal) {
  if (strobe && !signal) {
    controller_bits = controller_state;
  }

  strobe = signal;
}

auto Controller::peek(const usize port) const -> u8 {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1); // 1 == A
  }

  return 0x40 | (controller_bits[port] & 1);
}
} // namespace nes
