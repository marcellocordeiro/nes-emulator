#pragma once

#include <memory>
#include <vector>

#include "types/forward_decl.h"

namespace nes {
class emulator {
public:
  emulator();
  ~emulator();

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
  io*         get_io();
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
  std::unique_ptr<io>         io_ptr;
  std::unique_ptr<debugger>   debugger_ptr;

  std::vector<util::snapshotable*> snapshotable;
};
}  // namespace nes
