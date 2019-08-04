#include "mappers/mapper4.h"

#include "cartridge.h"

namespace nes {
mapper4::mapper4(nes::cartridge& cartridge_ref) : mapper{cartridge_ref} {}

void mapper4::reset()
{
  regs.fill(0);

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

  cartridge.set_mirroring(horizontal_mirroring ? Horizontal : Vertical);
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
        cartridge.set_cpu_irq(false);
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
    cartridge.set_cpu_irq(true);
  }
}

void mapper4::save(std::ofstream& out)
{
  for (const auto& value : prg_ram) dump_snapshot(out, value);
  for (const auto& value : chr) dump_snapshot(out, value);
  for (const auto& value : regs) dump_snapshot(out, value);

  dump_snapshot(out, reg_8000);
  dump_snapshot(out, horizontal_mirroring);
  dump_snapshot(out, irq_period, irq_counter, irq_enabled);
}

void mapper4::load(std::ifstream& in)
{
  for (auto& value : prg_ram) get_snapshot(in, value);
  for (auto& value : chr) get_snapshot(in, value);
  for (auto& value : regs) get_snapshot(in, value);

  get_snapshot(in, reg_8000);
  get_snapshot(in, horizontal_mirroring);
  get_snapshot(in, irq_period, irq_counter, irq_enabled);

  apply();
}
}  // namespace nes
