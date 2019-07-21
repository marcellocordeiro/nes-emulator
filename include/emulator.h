#pragma once

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "cpu.h"
#include "io.h"
#include "log.h"
#include "ppu.h"

#include "common.h"
#include "types/forward_decl.h"

namespace nes {
class emulator {
public:
  emulator();
  ~emulator() = default;

  void load_rom(const char*);
  void power_on();
  void run();

private:
  nes::cpu        cpu;
  nes::ppu        ppu;
  nes::apu        apu;
  nes::cartridge  cartridge;
  nes::controller controller;
  nes::io         io;
};
}  // namespace nes
