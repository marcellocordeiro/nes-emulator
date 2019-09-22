#pragma once

#include <memory>
#include <vector>

#include "utility/snapshotable.h"

namespace nes {
class cpu;
class apu;
class ppu;
class cartridge;
class base_mapper;
class controller;

class debugger;

class emulator {
public:
  emulator();
  ~emulator();

  emulator(const emulator&) = delete;
  emulator(emulator&&)      = delete;
  emulator& operator=(const emulator&) = delete;
  emulator& operator=(emulator&&) = delete;

  void load_rom();
  void power_on();
  void run();

  //
  // Component access
  //

  cpu*        get_cpu();
  ppu*        get_ppu();
  apu*        get_apu();
  cartridge*  get_cartridge();
  controller* get_controller();
  debugger*   get_debugger();

  //
  // Snapshot
  //

  void save_snapshot();
  void load_snapshot();

private:
  std::unique_ptr<cpu>        cpu_ptr;
  std::unique_ptr<ppu>        ppu_ptr;
  std::unique_ptr<apu>        apu_ptr;
  std::unique_ptr<cartridge>  cartridge_ptr;
  std::unique_ptr<controller> controller_ptr;
  std::unique_ptr<debugger>   debugger_ptr;

  std::vector<util::snapshotable*> snapshotable;
};
}  // namespace nes
