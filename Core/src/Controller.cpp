#include "Controller.h"

namespace nes {
Controller& Controller::get()
{
  static Controller instance;
  return instance;
}

void Controller::update_state(size_t port, uint8_t state) { controller_state[port] = state; }

uint8_t Controller::read(size_t port)
{
  if (strobe) {
    return 0x40 | (controller_state[port] & 1);  // 1 == A
  }

  uint8_t status        = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void Controller::write(bool signal)
{
  if (strobe && !signal) {
    controller_bits = controller_state;
  }

  strobe = signal;
}

uint8_t Controller::peek(size_t port) const
{
  if (strobe) {
    return 0x40 | (controller_state[port] & 1);  // 1 == A
  } else {
    return 0x40 | (controller_bits[port] & 1);
  }
}

void Controller::save(std::ofstream& out)
{
  dump_snapshot(out, controller_bits);
  dump_snapshot(out, strobe);
}

void Controller::load(std::ifstream& in)
{
  get_snapshot(in, controller_bits);
  get_snapshot(in, strobe);
}
}  // namespace nes