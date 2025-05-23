#include "base_mapper.hpp"

#include "lib/common.hpp"

namespace nes {
auto BaseMapper::get_mirroring() const -> MirroringType {
  return mirroring;
}

void BaseMapper::set_mirroring(const MirroringType value) {
  mirroring = value;
}

void BaseMapper::set_irq(const bool value) const {
  *irq_conn = value;
}

auto BaseMapper::get_prg_addr(const u16 addr) const -> usize {
  const usize slot = (addr - 0x8000u) / 0x2000u;
  const usize prg_addr = (addr - 0x8000u) % 0x2000u;

  return prg_map[slot] + prg_addr;
}

auto BaseMapper::get_chr_addr(const u16 addr) const -> usize {
  const usize slot = addr / 0x400;
  const usize chr_addr = addr % 0x400;

  return chr_map[slot] + chr_addr;
}

void BaseMapper::write(const u16 addr, const u8 value) {
  UNUSED(addr);
  UNUSED(value);
}

// Size must be in KB
template <std::size_t Size>
void BaseMapper::set_prg_map(const usize slot, i32 page) {
  constexpr usize pages = Size / 8;
  constexpr usize pages_b = Size * 0x400; // In bytes

  if (page < 0) {
    page += static_cast<i32>(prg_size) / static_cast<i32>(pages_b);
  }

  const auto resolved_page = static_cast<usize>(page);

  for (usize i = 0; i < pages; ++i) {
    prg_map[(pages * slot) + i] = ((pages_b * resolved_page) + 0x2000u * i) % prg_size;
  }
}

// Size must be in KB
template <std::size_t Size>
void BaseMapper::set_chr_map(const usize slot, const usize page) {
  constexpr usize pages = Size;
  constexpr usize pages_b = Size * 0x400; // In bytes

  for (usize i = 0; i < Size; ++i) {
    chr_map[(pages * slot) + i] = ((pages_b * page) + 0x400u * i) % chr_size;
  }
}

void BaseMapper::increment_scanline_counter() {}

// Explicit instantiation
template void BaseMapper::set_prg_map<32>(usize, i32);
template void BaseMapper::set_prg_map<16>(usize, i32);
template void BaseMapper::set_prg_map<8>(usize, i32);

template void BaseMapper::set_chr_map<8>(usize, usize);
template void BaseMapper::set_chr_map<4>(usize, usize);
template void BaseMapper::set_chr_map<2>(usize, usize);
template void BaseMapper::set_chr_map<1>(usize, usize);
} // namespace nes
