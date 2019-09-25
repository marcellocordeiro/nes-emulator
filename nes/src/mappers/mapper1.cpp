#include "nes/mappers/mapper1.h"

#include <nes/cartridge.h>

namespace nes {
mapper1::mapper1(cartridge& cart_ref) : base_mapper(cart_ref) {}

void mapper1::reset()
{
  write_delay = 5;
  shift_reg   = 0;

  control    = 0x0C;
  chr_bank_0 = 0;
  chr_bank_1 = 0;
  prg_bank   = 0;

  apply();
}

void mapper1::apply()
{
  using namespace mirroring;

  if (control & 0b1000) {
    // 16KB PRG-ROM
    if (control & 0b100) {
      // Switchable or fixed to the first bank
      set_prg_map<16>(0, prg_bank & 0xF);
      set_prg_map<16>(1, 0xF);
    } else {
      // Fixed to the last bank or switchable
      set_prg_map<16>(0, 0);
      set_prg_map<16>(1, prg_bank & 0xF);
    }
  } else {
    // 32KB PRG-ROM
    set_prg_map<32>(0, (prg_bank & 0xF) >> 1);
  }

  if (control & 0b10000) {
    // 4KB CHR-ROM
    set_chr_map<4>(0, chr_bank_0);
    set_chr_map<4>(1, chr_bank_1);
  } else {
    // 8KB CHR-ROM
    set_chr_map<8>(0, chr_bank_0 >> 1);
  }

  switch (control & 0b11) {
    case 0: cart.set_mirroring(One_Screen_Low); break;
    case 1: cart.set_mirroring(One_Screen_High); break;
    case 2: cart.set_mirroring(Vertical); break;
    case 3: cart.set_mirroring(Horizontal); break;
  }
}

void mapper1::prg_write(uint16_t addr, uint8_t value)
{
  if (addr < 0x8000) {
    prg_ram[addr - 0x6000] = value;
  } else if (addr & 0x8000) {
    if (value & 0x80) {  // Reset
      control |= 0x0C;
      write_delay = 5;
      shift_reg   = 0;

      apply();
    } else {
      shift_reg = ((value & 1) << 4) | (shift_reg >> 1);
      --write_delay;

      if (write_delay == 0) {
        switch ((addr >> 13) & 0x3) {
          case 0: control = shift_reg; break;
          case 1: chr_bank_0 = shift_reg; break;
          case 2: chr_bank_1 = shift_reg; break;
          case 3: prg_bank = shift_reg; break;
        }

        write_delay = 5;
        shift_reg   = 0;

        apply();
      }
    }
  }
}

void mapper1::save(std::ofstream& out)
{
  base_mapper::save(out);

  dump_snapshot(out, write_delay, shift_reg);
  dump_snapshot(out, control, chr_bank_0, chr_bank_1, prg_bank);
}

void mapper1::load(std::ifstream& in)
{
  base_mapper::load(in);

  get_snapshot(in, write_delay, shift_reg);
  get_snapshot(in, control, chr_bank_0, chr_bank_1, prg_bank);

  apply();
}
}  // namespace nes
