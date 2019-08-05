#include "controller.h"

#include "emulator.h"
#include "io.h"

namespace nes {
controller::controller(nes::emulator& emulator_ref) : emulator(emulator_ref) {}

uint8_t controller::read(size_t port)
{
  if (strobe) {
    return 0x40 | (emulator.get_io()->get_controller(port) & 1);  // 1 == A
  }

  uint8_t status        = 0x40 | (controller_bits[port] & 1);
  controller_bits[port] = 0x80 | (controller_bits[port] >> 1);

  return status;
}

void controller::write(bool signal)
{
  if (strobe && !signal) {
    controller_bits[0] = emulator.get_io()->get_controller(0);
    controller_bits[1] = emulator.get_io()->get_controller(1);
  }

  strobe = signal;
}

void controller::save(std::ofstream& out)
{
  for (const auto& value : controller_bits) dump_snapshot(out, value);
  dump_snapshot(out, strobe);
}

void controller::load(std::ifstream& in)
{
  for (auto& value : controller_bits) get_snapshot(in, value);
  get_snapshot(in, strobe);
}
}  // namespace nes
