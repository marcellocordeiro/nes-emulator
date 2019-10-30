#include "Cartridge.h"

#include <vector>

#include <spdlog/spdlog.h>

#include "Mappers/Mapper0.h"
#include "Mappers/Mapper1.h"
#include "Mappers/Mapper2.h"
#include "Mappers/Mapper4.h"
#include "Mappers/Mapper7.h"
#include "PPU.h"
#include "Utility/FileManager.h"

namespace nes {
Cartridge& Cartridge::get()
{
  static Cartridge instance;
  return instance;
}

BaseMapper* Cartridge::get_mapper() { return mapper.get(); }

void Cartridge::load()
{
  using namespace mirroring;

  auto rom = Utility::FileManager::get().get_rom();

  std::vector header(rom.begin(), rom.begin() + 16);

  size_t mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
  size_t prg_size     = header[4] * 0x4000;
  size_t chr_size     = header[5] * 0x2000;
  bool   chr_ram      = (chr_size == 0);
  size_t prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  int    mirroring    = (header[6] & 1) ? Vertical : Horizontal;

  switch (mapper_num) {
    case 0: mapper = std::make_unique<Mapper0>(); break;
    case 1: mapper = std::make_unique<Mapper1>(); break;
    case 2: mapper = std::make_unique<Mapper2>(); break;
    case 4: mapper = std::make_unique<Mapper4>(); break;
    case 7: mapper = std::make_unique<Mapper7>(); break;
    default:
      throw std::runtime_error(
          fmt::format("Mapper #{} not implemented", mapper_num));
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

  auto prg_ram = Utility::FileManager::get().get_prg_ram();

  if (chr_ram) {
    chr.resize(0x2000);
  }

  mapper->set_prg_rom(std::move(prg));
  mapper->set_chr_rom(std::move(chr));
  mapper->set_prg_ram(std::move(prg_ram));
  mapper->reset();

  PPU::get().set_mirroring(mirroring);
}

uint8_t Cartridge::prg_read(uint16_t addr) const
{
  return mapper->prg_read(addr);
}

void Cartridge::prg_write(uint16_t addr, uint8_t value)
{
  mapper->prg_write(addr, value);
}

uint8_t Cartridge::chr_read(uint16_t addr) const
{
  return mapper->chr_read(addr);
}

void Cartridge::chr_write(uint16_t addr, uint8_t value)
{
  mapper->chr_write(addr, value);
}

void Cartridge::scanline_counter() { mapper->scanline_counter(); }

void Cartridge::dump_prg_ram() const
{
  Utility::FileManager::get().save_prg_ram(mapper->get_prg_ram());
}
}  // namespace nes
