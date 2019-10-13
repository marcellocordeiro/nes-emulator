#include "nes/base_mapper.h"

#include <log.h>
#include <nes/cartridge.h>

namespace nes {
base_mapper::base_mapper(cartridge& cart_ref) : cart(cart_ref) {}

void base_mapper::set_prg_rom(std::vector<uint8_t>&& vec)
{
  prg = std::move(vec);
}

void base_mapper::set_chr_rom(std::vector<uint8_t>&& vec)
{
  chr = std::move(vec);
}

void base_mapper::set_prg_ram(std::vector<uint8_t>&& vec)
{
  prg_ram = std::move(vec);
}

std::vector<uint8_t> base_mapper::get_prg_ram() const { return prg_ram; }

uint8_t base_mapper::prg_read(uint16_t addr) const
{
  if (addr >= 0x8000) {
    size_t slot     = (addr - 0x8000) / 0x2000;
    size_t prg_addr = (addr - 0x8000) % 0x2000;

    return prg[prg_map[slot] + prg_addr];
  } else {
    return prg_ram[addr - 0x6000];
  }
}

uint8_t base_mapper::chr_read(uint16_t addr) const
{
  size_t slot     = addr / 0x400;
  size_t chr_addr = addr % 0x400;

  return chr[chr_map[slot] + chr_addr];
}

void base_mapper::prg_write(uint16_t addr, uint8_t value)
{
  prg_ram[addr] = value;
}

void base_mapper::chr_write(uint16_t addr, uint8_t value) { chr[addr] = value; }

// Size must be in KBs
template <size_t size> void base_mapper::set_prg_map(size_t slot, int page)
{
  constexpr size_t pages   = size / 8;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  if (page < 0) {
    page = (static_cast<int>(prg.size()) / pages_b) + page;
  }

  for (size_t i = 0; i < pages; ++i) {
    prg_map[pages * slot + i] = ((pages_b * page) + 0x2000 * i) % prg.size();
  }
}

template <size_t size> void base_mapper::set_chr_map(size_t slot, int page)
{
  constexpr size_t pages   = size;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  for (size_t i = 0; i < size; ++i) {
    chr_map[pages * slot + i] = ((pages_b * page) + 0x400 * i) % chr.size();
  }
}

template void base_mapper::set_prg_map<32>(size_t, int);
template void base_mapper::set_prg_map<16>(size_t, int);
template void base_mapper::set_prg_map<8>(size_t, int);

template void base_mapper::set_chr_map<8>(size_t, int);
template void base_mapper::set_chr_map<4>(size_t, int);
template void base_mapper::set_chr_map<2>(size_t, int);
template void base_mapper::set_chr_map<1>(size_t, int);

void base_mapper::scanline_counter() {}

void base_mapper::save(std::ofstream& out)
{
  dump_snapshot(out, prg_ram);
  dump_snapshot(out, chr);
}

void base_mapper::load(std::ifstream& in)
{
  get_snapshot(in, prg_ram);
  get_snapshot(in, chr);
}
}  // namespace nes
