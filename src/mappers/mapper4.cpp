#include "mappers/mapper4.h"

#include "cpu.h"

namespace nes {
mapper4::mapper4(
    const nes::cartridge_info& info_in,
    std::vector<uint8_t>&&     prg_in,
    std::vector<uint8_t>&&     chr_in)
  : mapper{info_in, std::move(prg_in), std::move(chr_in)}
{}

void mapper4::reset()
{
  for (int i = 0; i < 8; ++i) {
    regs[i] = 0;
  }

  horizontal_mirroring = true;
  irq_enabled          = false;
  irq_period           = 0;
  irq_counter          = 0;

  set_prg_map<8>(3, -1);
  apply();
}

void mapper4::apply()
{
  set_prg_map<8>(1, regs[7]);

  // PRG Mode 0
  if (!(reg_8000 & (1 << 6))) {
    set_prg_map<8>(0, regs[6]);
    set_prg_map<8>(2, -2);
  }

  // PRG Mode 1
  else {
    set_prg_map<8>(0, -2);
    set_prg_map<8>(2, regs[6]);
  }

  // CHR Mode 0
  if (!(reg_8000 & (1 << 7))) {
    set_chr_map<2>(0, regs[0] >> 1);
    set_chr_map<2>(1, regs[1] >> 1);

    for (int i = 0; i < 4; ++i) {
      set_chr_map<1>(4 + i, regs[2 + i]);
    }
  }

  // CHR Mode 1
  else {
    for (int i = 0; i < 4; ++i) {
      set_chr_map<1>(i, regs[2 + i]);
    }

    set_chr_map<2>(2, regs[0] >> 1);
    set_chr_map<2>(3, regs[1] >> 1);
  }

  using namespace mirroring;

  this->set_mirroring(horizontal_mirroring ? Horizontal : Vertical);
}

void mapper4::prg_write(uint16_t addr, uint8_t value)
{
  if (addr < 0x8000) {
    prg_ram[addr - 0x6000] = value;
  } else if (addr & 0x8000) {
    switch (addr & 0xE001) {
      case 0x8000: reg_8000 = value; break;
      case 0x8001: regs[reg_8000 & 7] = value; break;
      case 0xA000: horizontal_mirroring = value & 1; break;
      case 0xC000: irq_period = value; break;
      case 0xC001: irq_counter = 0; break;
      case 0xE000:
        this->bus->cpu.set_irq(false);
        irq_enabled = false;
        break;
      case 0xE001: irq_enabled = true; break;
    }

    apply();
  }
}

void mapper4::scanline_counter()
{
  if (irq_counter == 0) {
    irq_counter = irq_period;
  } else {
    --irq_counter;
  }

  if (irq_enabled && irq_counter == 0) {
    this->bus->cpu.set_irq();
  }
}
}  // namespace nes
