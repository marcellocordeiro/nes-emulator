#include <iostream>
#include <memory>
#include <stdexcept>

#include <system_utils.h>

#include "sdl2_frontend.h"

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Too few arguments" << '\n';
    //return 1;
  }

  try {
    auto frontend = std::make_unique<nes::sdl2_frontend>(argc, argv);

    frontend->init();
    frontend->run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }
}
