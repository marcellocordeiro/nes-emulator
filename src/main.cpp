#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "emulator.h"
#include "system_utils.h"

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Too few arguments" << '\n';
    return 1;
    // throw std::invalid_argument("Too few arguments");
  }

  // Prevents pointer invalidation
  auto path_str = lib::get_executable_path().string();
  app_path      = path_str.c_str();

  std::ofstream log_file{std::filesystem::path(app_path) / "nes-emulator.log"};
  lib::log::get().set_stream(log_file);

  nes::emulator emulator;

  try {
    emulator.load_rom(argv[1]);
    emulator.power_on();

    emulator.run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }
}
