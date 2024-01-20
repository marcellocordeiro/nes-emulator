#include "controller.h"

namespace nes {
auto Controller::get() -> Controller& {
  static Controller instance;
  return instance;
}

void Controller::update_state(size_t port, uint8_t state) {
  controller_state[port] = state;
}

auto Controller::read(size_t port) -> uint8_t {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1);  // 1 == A
  }

  uint8_t status = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void Controller::write(bool signal) {
  if (strobe && !signal) {
    controller_bits = controller_state;
  }

  strobe = signal;
}

auto Controller::peek(size_t port) const -> uint8_t {
  if (strobe) {
    return 0x40 | (controller_state[port] & 1);  // 1 == A
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
}  // namespace nes
