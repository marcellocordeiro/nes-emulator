#include "BaseMapper.h"
#include "CPU.h"
#include "PPU.h"
#include "Types/Cartridge_Types.h"

namespace nes {
void BaseMapper::set_mirroring(types::cartridge::mirroring_type value)
{
  mirroring = value;
  //PPU::get().set_mirroring(mirroring);
  mirroring_conn->set(value);
}

void BaseMapper::set_irq(bool value) { irq_conn->set(value); }

void BaseMapper::set_prg_rom(std::vector<uint8_t>&& vec) { prg = std::move(vec); }

void BaseMapper::set_chr_rom(std::vector<uint8_t>&& vec) { chr = std::move(vec); }

void BaseMapper::set_prg_ram(std::vector<uint8_t>&& vec) { prg_ram = std::move(vec); }

auto BaseMapper::get_prg_ram() const -> std::vector<uint8_t> { return prg_ram; }

auto BaseMapper::prg_read(uint16_t addr) const -> uint8_t
{
  // PRG-ROM
  if (addr >= 0x8000) {
    size_t slot     = (addr - 0x8000) / 0x2000;
    size_t prg_addr = (addr - 0x8000) % 0x2000;

    return prg[prg_map[slot] + prg_addr];
  }

  // PRG-RAM
  return prg_ram[addr - 0x6000];
}

auto BaseMapper::chr_read(uint16_t addr) const -> uint8_t
{
  size_t slot     = addr / 0x400;
  size_t chr_addr = addr % 0x400;

  return chr[chr_map[slot] + chr_addr];
}

void BaseMapper::prg_write(uint16_t addr, uint8_t value) { prg_ram[addr - 0x6000] = value; }

void BaseMapper::chr_write(uint16_t addr, uint8_t value) { chr[addr] = value; }

// Size must be in KB
template <size_t size> void BaseMapper::set_prg_map(size_t slot, int page)
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

// Size must be in KB
template <size_t size> void BaseMapper::set_chr_map(size_t slot, int page)
{
  constexpr size_t pages   = size;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  for (size_t i = 0; i < size; ++i) {
    chr_map[pages * slot + i] = ((pages_b * page) + 0x400 * i) % chr.size();
  }
}

void BaseMapper::scanline_counter() {}

void BaseMapper::save(std::ofstream& out)
{
  dump_snapshot(out, prg_ram);
  dump_snapshot(out, chr);
}

void BaseMapper::load(std::ifstream& in)
{
  get_snapshot(in, prg_ram);
  get_snapshot(in, chr);
}

// Explicit instantiation
template void BaseMapper::set_prg_map<32>(size_t, int);
template void BaseMapper::set_prg_map<16>(size_t, int);
template void BaseMapper::set_prg_map<8>(size_t, int);

template void BaseMapper::set_chr_map<8>(size_t, int);
template void BaseMapper::set_chr_map<4>(size_t, int);
template void BaseMapper::set_chr_map<2>(size_t, int);
template void BaseMapper::set_chr_map<1>(size_t, int);
}  // namespace nes
