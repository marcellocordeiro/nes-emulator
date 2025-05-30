#include "cartridge.hpp"

#include <algorithm>
#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <span>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "base_mapper.hpp"
#include "lib/common.hpp"
#include "mappers/mapper_0.hpp"
#include "mappers/mapper_1.hpp"
#include "mappers/mapper_2.hpp"
#include "mappers/mapper_4.hpp"
#include "mappers/mapper_7.hpp"
#include "types/ppu_types.hpp"

namespace nes {
auto Cartridge::get() -> Cartridge& {
  static Cartridge instance;
  return instance;
}

auto Cartridge::get_mapper() const -> BaseMapper* {
  return mapper.get();
}

auto Cartridge::get_mirroring() const -> MirroringType {
  return mapper->get_mirroring();
}

void Cartridge::load(
  const std::vector<u8>& rom,
  const std::optional<std::vector<u8>>& prg_ram_file,
  std::shared_ptr<bool> irq
) {
  const std::span header(rom.begin(), rom.begin() + 16);

  const usize mapper_num = (header[7] & 0xF0) | (header[6] >> 4);
  const usize prg_size = static_cast<usize>(header[4]) * 0x4000;
  const usize chr_size = static_cast<usize>(header[5]) * 0x2000;
  const bool has_chr_ram = chr_size == 0;
  const usize prg_ram_size = header[8] != 0 ? header[8] * 0x2000 : 0x2000;
  const auto mirroring =
    (header[6] & 0b1) != 0 ? MirroringType::Vertical : MirroringType::Horizontal;

  // PRG
  constexpr usize prg_start = 16;

  prg = rom
    | std::ranges::views::drop(prg_start)
    | std::ranges::views::take(prg_size)
    | std::ranges::to<std::vector>();

  // CHR
  if (!has_chr_ram) {
    const usize chr_start = prg_start + prg_size;

    chr = rom
      | std::ranges::views::drop(chr_start)
      | std::ranges::views::take(chr_size)
      | std::ranges::to<std::vector>();
  } else {
    chr.resize(0x2000);
  }

  // PRG-RAM
  if (prg_ram_file) {
    prg_ram = *prg_ram_file;
  } else {
    prg_ram.resize(0x2000);
  }

  switch (mapper_num) {
    case 0: mapper = std::make_unique<Mapper0>(); break;
    case 1: mapper = std::make_unique<Mapper1>(); break;
    case 2: mapper = std::make_unique<Mapper2>(); break;
    case 4: mapper = std::make_unique<Mapper4>(); break;
    case 7: mapper = std::make_unique<Mapper7>(); break;
    default: throw std::runtime_error(std::format("Mapper #{} not implemented", mapper_num));
  }

  mapper->prg_size = prg.size();
  mapper->chr_size = chr.size();
  mapper->irq = std::move(irq);
  mapper->set_mirroring(mirroring);
  mapper->reset();

  spdlog::info("PRG-ROM size (16KB banks): {}", prg_size);
  spdlog::info("CHR-ROM size (8KB banks): {}", chr_size);
  spdlog::info("Mirroring: {}", types::ppu::get_mirroring_name(mirroring));
  spdlog::info("Mapper #: {}", mapper_num);
  spdlog::info("PRG RAM size: {}", prg_ram_size);
}

auto Cartridge::prg_read(const u16 addr) const -> u8 {
  if (addr < 0x8000) {
    return prg_ram[addr - 0x6000];
  }

  const usize mapped_addr = mapper->get_prg_addr(addr);
  return prg[mapped_addr];
}

void Cartridge::prg_write(const u16 addr, const u8 value) {
  if (addr < 0x8000) {
    prg_ram[addr - 0x6000] = value;
    return;
  }

  mapper->write(addr, value);
}

auto Cartridge::chr_read(const u16 addr) const -> u8 {
  const usize mapped_addr = mapper->get_chr_addr(addr);
  return chr[mapped_addr];
}

void Cartridge::chr_write(const u16 addr, const u8 value) {
  chr[addr] = value;
}

void Cartridge::scanline_counter() const {
  mapper->increment_scanline_counter();
}

auto Cartridge::get_prg_ram() const -> std::vector<u8> {
  return prg_ram;
}
} // namespace nes
