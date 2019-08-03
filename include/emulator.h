#pragma once

#include <memory>
#include <vector>

#include "types/forward_decl.h"

namespace nes {
class emulator {
public:
  emulator();
  ~emulator();

  void load_rom(const char*);
  void power_on();
  void run();

  nes::cpu*        get_cpu();
  nes::ppu*        get_ppu();
  nes::apu*        get_apu();
  nes::cartridge*  get_cartridge();
  nes::controller* get_controller();
  nes::io*         get_io();

  void save_snapshot();
  void load_snapshot();

private:
  std::unique_ptr<nes::cpu>        cpu;
  std::unique_ptr<nes::ppu>        ppu;
  std::unique_ptr<nes::apu>        apu;
  std::unique_ptr<nes::cartridge>  cartridge;
  std::unique_ptr<nes::controller> controller;
  std::unique_ptr<nes::io>         io;

  std::vector<nes::util::snapshotable*> snapshotable;
};
}  // namespace nes
