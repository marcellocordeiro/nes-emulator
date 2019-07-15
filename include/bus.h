#pragma once

namespace nes {
class cpu;
class apu;
class ppu;
class cartridge;
class controller;

class debugger;
class emulator;

class bus {
public:
  nes::cpu&        cpu;
  nes::ppu&        ppu;
  nes::apu&        apu;
  nes::cartridge&  cartridge;
  nes::controller& controller;

  // nes::debugger&   debugger;
  nes::emulator& emulator;
};
}  // namespace nes
