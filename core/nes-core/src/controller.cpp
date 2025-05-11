#include "controller.hpp"

namespace nes {
auto Controller::get() -> Controller& {
  static Controller instance;
  return instance;
}

void Controller::update_state(usize port, u8 state) {
  controller_state[port] = state;
}

auto Controller::read(usize port) -> u8 {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1); // 1 == A
  }

  const u8 status = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void Controller::write(bool signal) {
  if (strobe && !signal) {
    controller_bits = controller_state;
  }

  strobe = signal;
}

auto Controller::peek(usize port) const -> u8 {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1); // 1 == A
  }

  return 0x40 | (controller_bits[port] & 1);
}

void Controller::save(std::ofstream& out) const {
  dump_snapshot(out, controller_bits);
  dump_snapshot(out, strobe);
}

void Controller::load(std::ifstream& in) {
  get_snapshot(in, controller_bits);
  get_snapshot(in, strobe);
}
} // namespace nes
