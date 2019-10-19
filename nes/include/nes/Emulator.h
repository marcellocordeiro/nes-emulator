#pragma once

#include <memory>
#include <vector>

#include "../../src/APU.h"
#include "../../src/BaseMapper.h"
#include "../../src/Cartridge.h"
#include "../../src/Controller.h"
#include "../../src/CPU.h"
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

  //
  // Component access
  //

  //
  // Snapshot
  //

  void save_snapshot();
  void load_snapshot();

  static constexpr int width  = 256;
  static constexpr int height = 240;

private:
  Emulator() = default;

  std::vector<util::snapshotable*> snapshotable;
};
}  // namespace nes
