#include "base_mapper.hpp"

#include "lib/common.hpp"

namespace nes {
void BaseMapper::set_mirroring(MirroringType value) {
  // TODO: save this?
  // mirroring = value;
  *mirroring_conn = value;
}

void BaseMapper::set_irq(bool value) {
  *irq_conn = value;
}

auto BaseMapper::get_prg_addr(u16 addr) const -> usize {
  usize slot = (addr - 0x8000) / 0x2000;
  usize prg_addr = (addr - 0x8000) % 0x2000;

  return prg_map[slot] + prg_addr;
}

auto BaseMapper::get_chr_addr(u16 addr) const -> usize {
  usize slot = addr / 0x400;
  usize chr_addr = addr % 0x400;

  return chr_map[slot] + chr_addr;
}

void BaseMapper::write(u16 addr, u8 value) {
  UNUSED(addr);
  UNUSED(value);
}

// Size must be in KB
template <usize size>
void BaseMapper::set_prg_map(usize slot, i32 page) {
  constexpr usize pages = size / 8;
  constexpr usize pages_b = size * 0x400; // In bytes

  if (page < 0) {
    page = (static_cast<i32>(prg_size) / pages_b) + page;
  }

  for (usize i = 0; i < pages; ++i) {
    prg_map[(pages * slot) + i] = ((pages_b * page) + 0x2000 * i) % prg_size;
  }
}

// Size must be in KB
template <usize size>
void BaseMapper::set_chr_map(usize slot, i32 page) {
  constexpr usize pages = size;
  constexpr usize pages_b = size * 0x400; // In bytes

  for (usize i = 0; i < size; ++i) {
    chr_map[(pages * slot) + i] = ((pages_b * page) + 0x400 * i) % chr_size;
  }
}

void BaseMapper::scanline_counter() {}

// TODO
void BaseMapper::save(std::ofstream& out) const {
  UNUSED(out);
  // dump_snapshot(out, prg_ram);
  // dump_snapshot(out, chr);
}

void BaseMapper::load(std::ifstream& in) {
  UNUSED(in);
  // get_snapshot(in, prg_ram);
  // get_snapshot(in, chr);
}

// Explicit instantiation
template void BaseMapper::set_prg_map<32>(usize, i32);
template void BaseMapper::set_prg_map<16>(usize, i32);
template void BaseMapper::set_prg_map<8>(usize, i32);

template void BaseMapper::set_chr_map<8>(usize, i32);
template void BaseMapper::set_chr_map<4>(usize, i32);
template void BaseMapper::set_chr_map<2>(usize, i32);
template void BaseMapper::set_chr_map<1>(usize, i32);
} // namespace nes
