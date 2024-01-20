#include "base_mapper.h"

namespace nes {
void BaseMapper::set_mirroring(MirroringType value) {
  // TODO: save this?
  // mirroring = value;
  *mirroring_conn = value;
}

void BaseMapper::set_irq(bool value) { *irq_conn = value; }

auto BaseMapper::get_prg_addr(uint16_t addr) const -> size_t {
  size_t slot = (addr - 0x8000) / 0x2000;
  size_t prg_addr = (addr - 0x8000) % 0x2000;

  return prg_map[slot] + prg_addr;
}

auto BaseMapper::get_chr_addr(uint16_t addr) const -> size_t {
  size_t slot = addr / 0x400;
  size_t chr_addr = addr % 0x400;

  return chr_map[slot] + chr_addr;
}

void BaseMapper::write(uint16_t addr, uint8_t value) {
  UNUSED(addr);
  UNUSED(value);
}

// Size must be in KB
template <size_t size>
void BaseMapper::set_prg_map(size_t slot, int page) {
  constexpr size_t pages = size / 8;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  if (page < 0) {
    page = (static_cast<int>(prg_size) / pages_b) + page;
  }

  for (size_t i = 0; i < pages; ++i) {
    prg_map[pages * slot + i] = ((pages_b * page) + 0x2000 * i) % prg_size;
  }
}

// Size must be in KB
template <size_t size>
void BaseMapper::set_chr_map(size_t slot, int page) {
  constexpr size_t pages = size;
  constexpr size_t pages_b = size * 0x400;  // In bytes

  for (size_t i = 0; i < size; ++i) {
    chr_map[pages * slot + i] = ((pages_b * page) + 0x400 * i) % chr_size;
  }
}

void BaseMapper::scanline_counter() {}

// TODO
void BaseMapper::save(std::ofstream& /*out*/) const {
  // dump_snapshot(out, prg_ram);
  // dump_snapshot(out, chr);
}

void BaseMapper::load(std::ifstream& /*in*/) {
  // get_snapshot(in, prg_ram);
  // get_snapshot(in, chr);
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
