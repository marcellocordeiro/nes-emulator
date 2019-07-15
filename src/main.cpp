#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "apu.h"
#include "bus.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "emulator.h"
#include "log.h"
#include "ppu.h"

int main(int argc, char* argv[])
{
  if (argc == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  app_path = argv[0];

  std::ofstream log_file{std::filesystem::path(app_path).parent_path() /
                         "nes-emulator.log"};
  lib::log::get().set_stream(log_file);

  nes::cpu        cpu;
  nes::ppu        ppu;
  nes::apu        apu;
  nes::cartridge  cartridge;
  nes::controller controller;
  nes::emulator   emulator;

  nes::bus bus{cpu, ppu, apu, cartridge, controller, emulator};

  cpu.set_bus(bus);
  ppu.set_bus(bus);
  apu.set_bus(bus);
  cartridge.set_bus(bus);
  controller.set_bus(bus);
  emulator.set_bus(bus);

  cartridge.load(argv[1]);
  cpu.power_on();
  ppu.power_on();
  apu.power_on();

  try {
    emulator.run();
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
  }
}
