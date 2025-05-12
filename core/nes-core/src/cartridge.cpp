#include "cartridge.hpp"

#include <format>
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
#include "utility/file_manager.hpp"

namespace nes {
auto Cartridge::get() -> Cartridge& {
  static Cartridge instance;
  return instance;
}

auto Cartridge::get_mapper() const -> BaseMapper* {
  return mapper.get();
}

void Cartridge::load() {
  rom = utility::FileManager::get().get_rom();

  const std::span header(rom.begin(), rom.begin() + 16);

  mapper_num = (header[7] & 0xF0) | (header[6] >> 4);
  prg_size = header[4] * 0x4000;
  chr_size = header[5] * 0x2000;
  has_chr_ram = chr_size == 0;
  prg_ram_size = header[8] != 0 ? header[8] * 0x2000 : 0x2000;
  mirroring = (header[6] & 1) != 0 ? MirroringType::Vertical : MirroringType::Horizontal;

  switch (mapper_num) {
  case 0: mapper = std::make_unique<Mapper0>(); break;
  case 1: mapper = std::make_unique<Mapper1>(); break;
  case 2: mapper = std::make_unique<Mapper2>(); break;
  case 4: mapper = std::make_unique<Mapper4>(); break;
  case 7: mapper = std::make_unique<Mapper7>(); break;
  default: throw std::runtime_error(std::format("Mapper #{} not implemented", mapper_num));
  }

  auto prg_start = rom.begin() + 16;
  auto prg_end = prg_start + prg_size;

  prg = {prg_start, prg_end};

  if (!has_chr_ram) {
    auto chr_start = prg_end;
    auto chr_end = chr_start + chr_size;

    chr = {chr_start, chr_end};
  } else {
    chr_ram.resize(0x2000);

    chr = chr_ram;
  }

  if (utility::FileManager::get().has_prg_ram()) {
    prg_ram = utility::FileManager::get().get_prg_ram();
  } else {
    prg_ram.resize(0x2000, 0);
  }

  mapper->prg_size = prg.size();
  mapper->chr_size = chr.size();
  mapper->mirroring_conn = mirroring_conn;
  mapper->irq_conn = irq_conn;
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
  mapper->scanline_counter();
}

void Cartridge::dump_prg_ram() const {
  utility::FileManager::get().save_prg_ram(prg_ram);
}
} // namespace nes
