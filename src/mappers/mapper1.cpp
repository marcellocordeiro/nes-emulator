#include "mappers/mapper1.h"

#include "cartridge.h"

namespace nes {
mapper1::mapper1(nes::cartridge& cartridge_ref) : mapper{cartridge_ref} {}

void mapper1::reset()
{
  write_counter = 0;
  shift_reg     = 0;

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
    case 0: cartridge.set_mirroring(One_Screen_Low); break;
    case 1: cartridge.set_mirroring(One_Screen_High); break;
    case 2: cartridge.set_mirroring(Vertical); break;
    case 3: cartridge.set_mirroring(Horizontal); break;
  }
}

void mapper1::prg_write(const uint16_t addr, const uint8_t value)
{
  if (addr < 0x8000) {
    prg_ram[addr - 0x6000] = value;
  } else if (addr & 0x8000) {
    if (value & 0x80) {  // Reset
      control |= 0x0C;
      write_counter = 0;
      shift_reg     = 0;

      apply();
    } else {
      shift_reg = ((value & 1) << 4) | (shift_reg >> 1);
      ++write_counter;

      if (write_counter == 5) {
        switch ((addr >> 13) & 0x3) {
          case 0: control = shift_reg; break;
          case 1: chr_bank_0 = shift_reg; break;
          case 2: chr_bank_1 = shift_reg; break;
          case 3: prg_bank = shift_reg; break;
        }

        write_counter = 0;
        shift_reg     = 0;

        apply();
      }
    }
  }
}

void mapper1::save(std::ofstream& out)
{
  for (const auto& value : prg_ram) dump_snapshot(out, value);
  dump_snapshot(out, write_counter, shift_reg);
  dump_snapshot(out, control, chr_bank_0, chr_bank_1, prg_bank);
}

void mapper1::load(std::ifstream& in)
{
  for (auto& value : prg_ram) get_snapshot(in, value);
  get_snapshot(in, write_counter, shift_reg);
  get_snapshot(in, control, chr_bank_0, chr_bank_1, prg_bank);
}
}  // namespace nes
