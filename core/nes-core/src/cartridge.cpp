#include "cartridge.h"

#include <format>

#include <spdlog/spdlog.h>

#include "mappers/mapper_0.h"
#include "mappers/mapper_1.h"
#include "mappers/mapper_2.h"
#include "mappers/mapper_4.h"
#include "mappers/mapper_7.h"
#include "utility/file_manager.h"

namespace nes {
auto Cartridge::get() -> Cartridge& {
  static Cartridge instance;
  return instance;
}

auto Cartridge::get_mapper() -> BaseMapper* {
  return mapper.get();
}

void Cartridge::load() {
  rom = utility::FileManager::get().get_rom();

  std::span header(rom.begin(), rom.begin() + 16);

  mapper_num = (header[7] & 0xF0) | (header[6] >> 4);
  prg_size = header[4] * 0x4000;
  chr_size = header[5] * 0x2000;
  has_chr_ram = chr_size == 0;
  prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  mirroring = (header[6] & 1) ? MirroringType::Vertical : MirroringType::Horizontal;

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

auto Cartridge::prg_read(uint16_t addr) const -> uint8_t {
  if (addr < 0x8000) {
    return prg_ram[addr - 0x6000];
  }

  size_t mapped_addr = mapper->get_prg_addr(addr);
  return prg[mapped_addr];
}

void Cartridge::prg_write(uint16_t addr, uint8_t value) {
  if (addr < 0x8000) {
    prg_ram[addr - 0x6000] = value;
    return;
  }

  mapper->write(addr, value);
}

auto Cartridge::chr_read(uint16_t addr) const -> uint8_t {
  size_t mapped_addr = mapper->get_chr_addr(addr);
  return chr[mapped_addr];
}

void Cartridge::chr_write(uint16_t addr, uint8_t value) {
  chr[addr] = value;
}

void Cartridge::scanline_counter() {
  mapper->scanline_counter();
}

void Cartridge::dump_prg_ram() const {
  utility::FileManager::get().save_prg_ram(prg_ram);
}
} // namespace nes
