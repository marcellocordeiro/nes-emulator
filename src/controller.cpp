#include "controller.h"

#include "io.h"

namespace nes {
controller::controller(nes::io& io_ref) : io(io_ref) {}

uint8_t controller::read(size_t port)
{
  if (strobe) {
    return 0x40 | (io.get_controller(port) & 1);  // 1 == A
  }

  uint8_t status        = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void controller::write(bool signal)
{
  if (strobe && !signal) {
    controller_bits[0] = io.get_controller(0);
    controller_bits[1] = io.get_controller(1);
  }

  strobe = signal;
}
}  // namespace nes
