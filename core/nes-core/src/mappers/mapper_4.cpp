#include "mapper_4.hpp"

#include <iosfwd>

#include <spdlog/spdlog.h>

#include "../base_mapper.hpp"
#include "lib/common.hpp"

namespace nes {
void Mapper4::reset() {
  regs.fill(0);
  reg_8000 = 0;

  irq_enabled = false;
  irq_period = 0;
  irq_counter = 0;

  set_mirroring(MirroringType::Horizontal);
  set_prg_map<8>(3, -1);
  apply();
}

void Mapper4::apply() {
  set_prg_map<8>(1, regs[7]);

  if ((reg_8000 & (1 << 6)) == 0) {
    // PRG Mode 0
    set_prg_map<8>(0, regs[6]);
    set_prg_map<8>(2, -2);
  } else {
    // PRG Mode 1
    set_prg_map<8>(0, -2);
    set_prg_map<8>(2, regs[6]);
  }

  if ((reg_8000 & (1 << 7)) == 0) {
    // CHR Mode 0
    set_chr_map<2>(0, regs[0] >> 1);
    set_chr_map<2>(1, regs[1] >> 1);

    for (usize i = 0; i < 4; ++i) {
      set_chr_map<1>(4 + i, regs[2 + i]);
    }
  } else {
    // CHR Mode 1
    for (usize i = 0; i < 4; ++i) {
      set_chr_map<1>(i, regs[2 + i]);
    }

    set_chr_map<2>(2, regs[0] >> 1);
    set_chr_map<2>(3, regs[1] >> 1);
  }
}

void Mapper4::write(const u16 addr, const u8 value) {
  if (addr < 0x8000) {
    // prg_ram[addr - 0x6000] = value;
  } else if ((addr & 0x8000) != 0) {
    switch (addr & 0xE001) {
    case 0x8000: reg_8000 = value; break;
    case 0x8001: regs[reg_8000 & 7] = value; break;
    case 0xA000:
      set_mirroring((value & 1) != 0 ? MirroringType::Horizontal : MirroringType::Vertical);
      break;
    case 0xC000: irq_period = value; break;
    case 0xC001: irq_counter = 0; break;
    case 0xE000:
      set_irq(false);
      irq_enabled = false;
      break;
    case 0xE001: irq_enabled = true; break;
    }

    apply();
  }
}

void Mapper4::scanline_counter() {
  if (irq_counter == 0) {
    irq_counter = irq_period;
  } else {
    --irq_counter;
  }

  if (irq_enabled && irq_counter == 0) {
    set_irq(true);
  }
}

void Mapper4::save(std::ofstream& out) const {
  BaseMapper::save(out);

  dump_snapshot(out, regs);
  dump_snapshot(out, reg_8000);
  dump_snapshot(out, horizontal_mirroring);
  dump_snapshot(out, irq_period, irq_counter, irq_enabled);
}

void Mapper4::load(std::ifstream& in) {
  BaseMapper::load(in);

  get_snapshot(in, regs);
  get_snapshot(in, reg_8000);
  get_snapshot(in, horizontal_mirroring);
  get_snapshot(in, irq_period, irq_counter, irq_enabled);

  apply();
}
} // namespace nes
