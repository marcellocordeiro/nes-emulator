#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "emulator.h"
#include "system_utils.h"
#include "utility/file_manager.h"

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Too few arguments" << '\n';
    return 1;
  }

  auto main_path = lib::get_executable_path();

  std::ofstream log_file{main_path / "nes-emulator.log"};
  lib::log::get().set_stream(log_file);

  try {
    nes::util::fmngr.set_rom(argv[1]);
    nes::util::fmngr.set_palette(main_path / "palette.pal");

    nes::emulator emulator;

    emulator.load_rom(argv[1]);
    emulator.power_on();

    emulator.run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }
}
