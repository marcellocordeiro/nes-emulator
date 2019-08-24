#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "emulator.h"
#include "log.h"
#include "system_utils.h"
#include "utility/file_manager.h"

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Too few arguments" << '\n';
    return 1;
  }

  try {
    auto app_path = nes::util::fmngr.get_app_path();

    // std::ofstream log_file{app_path / "nes-emulator.log"};
    // lib::log::get().set_stream(log_file);

    nes::util::fmngr.set_rom(argv[1]);
    nes::util::fmngr.set_palette(app_path / "palette.pal");

    nes::emulator emulator;

    emulator.load_rom("");
    emulator.power_on();

    emulator.run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }
}
