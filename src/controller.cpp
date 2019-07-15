#include "controller.h"

#include "emulator.h"

namespace nes {
void controller::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

uint8_t controller::read(size_t port)
{
  if (strobe) {
    return 0x40 | (this->bus->emulator.get_controller(port) & 1);  // 1 == A
  }

  uint8_t status = (controller_bits[port] & 1) | 0x40;
  controller_bits[port] >>= 1;

  return status;
}

void controller::write(bool signal)
{
  if (strobe && !signal) {
    controller_bits[0] = this->bus->emulator.get_controller(0);
    controller_bits[1] = this->bus->emulator.get_controller(1);
  }

  strobe = signal;
}
}  // namespace nes
