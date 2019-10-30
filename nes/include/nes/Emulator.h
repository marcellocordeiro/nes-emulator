#pragma once

#include <memory>
#include <vector>

#include "../../src/APU.h"
#include "../../src/BaseMapper.h"
#include "../../src/CPU.h"
#include "../../src/Cartridge.h"
#include "../../src/Controller.h"
#include "../../src/PPU.h"
#include "../../src/Utility/FileManager.h"
#include "../../src/Utility/Snapshotable.hpp"

namespace nes {
class Emulator {
public:
  Emulator(const Emulator&) = delete;
  Emulator(Emulator&&)      = delete;
  Emulator& operator=(const Emulator&) = delete;
  Emulator& operator=(Emulator&&) = delete;

  static Emulator& get();

  void power_on();
  void reset();

  //
  // Component access
  //

  void run_frame();
  void volume(double);

  //
  // Snapshot
  //

  void save_snapshot();
  void load_snapshot();

  static constexpr int width  = 256;
  static constexpr int height = 240;

  static constexpr const char* title = "nes-emulator";

private:
  Emulator() = default;

  std::vector<Utility::Snapshotable*> snapshotable;
};
}  // namespace nes
