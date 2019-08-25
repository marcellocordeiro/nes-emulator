#include <iostream>
#include <stdexcept>

#include "emulator.h"
#include "system_utils.h"
#include "utility/file_manager.h"

int main(int argc, [[maybe_unused]] char* argv[])
{
  if (argc == 1) {
    std::cout << "Too few arguments" << '\n';
    return 1;
  }

  try {
    nes::util::fmngr.setup();
    nes::util::fmngr.set_rom(argv[1]);

    // std::ofstream log_file{app_path / "nes-emulator.log"};
    // lib::log::get().set_stream(log_file);

    nes::emulator emulator;

    emulator.load_rom();
    emulator.power_on();

    emulator.run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }
}
