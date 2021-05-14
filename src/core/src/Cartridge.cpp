#include "Cartridge.h"

#include <vector>

#include <spdlog/spdlog.h>

#include "Mappers/Mapper0.h"
#include "Mappers/Mapper1.h"
#include "Mappers/Mapper2.h"
#include "Mappers/Mapper4.h"
#include "Mappers/Mapper7.h"
#include "PPU.h"
#include "Types/Cartridge_Types.h"
#include "Utility/FileManager.h"

namespace nes {
using namespace types::cartridge;

auto Cartridge::get() -> Cartridge&
{
  static Cartridge instance;
  return instance;
}

auto Cartridge::get_mapper() -> BaseMapper* { return mapper.get(); }

void Cartridge::load()
{
  auto rom = Utility::FileManager::get().get_rom();

  std::vector header(rom.begin(), rom.begin() + 16);

  mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
  prg_size     = header[4] * 0x4000;
  chr_size     = header[5] * 0x2000;
  chr_ram      = chr_size == 0;
  prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  mirroring    = (header[6] & 1) ? Vertical : Horizontal;

  switch (mapper_num) {
    case 0: mapper = std::make_unique<Mapper0>(); break;
    case 1: mapper = std::make_unique<Mapper1>(); break;
    case 2: mapper = std::make_unique<Mapper2>(); break;
    case 4: mapper = std::make_unique<Mapper4>(); break;
    case 7: mapper = std::make_unique<Mapper7>(); break;
    default: throw std::runtime_error(fmt::format("Mapper #{} not implemented", mapper_num));
  }

  spdlog::info("PRG-ROM size (16KB banks): {}", prg_size);
  spdlog::info("CHR-ROM size (8KB banks): {}", chr_size);
  spdlog::info("Mirroring: {}", (mirroring ? "Vertical" : "Horizontal"));
  spdlog::info("Mapper #: {}", mapper_num);
  spdlog::info("PRG RAM size: {}", prg_ram_size);

  auto prg_start = 16;
  auto prg_end   = 16 + prg_size;
  auto chr_start = prg_end;
  auto chr_end   = prg_end + chr_size;

  std::vector prg(rom.begin() + prg_start, rom.begin() + prg_end);
  std::vector chr(rom.begin() + chr_start, rom.begin() + chr_end);

  if (chr_ram) {
    chr.resize(0x2000);
  }

  mapper->set_mirroring(mirroring);
  mapper->set_prg_rom(std::move(prg));
  mapper->set_chr_rom(std::move(chr));

  if (Utility::FileManager::get().has_prg_ram()) {
    mapper->set_prg_ram(Utility::FileManager::get().get_prg_ram());
  } else {
    mapper->set_prg_ram(std::vector(0x2000, uint8_t{}));
  }

  mapper->reset();
}

auto Cartridge::prg_read(uint16_t addr) const -> uint8_t { return mapper->prg_read(addr); }

void Cartridge::prg_write(uint16_t addr, uint8_t value) { mapper->prg_write(addr, value); }

auto Cartridge::chr_read(uint16_t addr) const -> uint8_t { return mapper->chr_read(addr); }

void Cartridge::chr_write(uint16_t addr, uint8_t value) { mapper->chr_write(addr, value); }

void Cartridge::scanline_counter() { mapper->scanline_counter(); }

void Cartridge::dump_prg_ram() const { Utility::FileManager::get().save_prg_ram(mapper->get_prg_ram()); }
}  // namespace nes