#include "cartridge.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "log.h"
#include "mappers/mapper0.h"
#include "mappers/mapper1.h"
#include "mappers/mapper2.h"
#include "mappers/mapper4.h"

namespace nes {
void cartridge::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

void cartridge::load(const char* rom_path)
{
  using namespace mirroring;

  auto rom_file = std::filesystem::path(rom_path);

  std::ifstream rom(rom_file, std::ios::binary);

  if (!rom) {
    throw std::runtime_error("Can't open the ROM");
  }

  std::array<uint8_t, 16> header;
  rom.read(reinterpret_cast<char*>(header.data()), 16);

  info.path         = rom_path;
  info.rom_size     = std::filesystem::file_size(rom_file);
  info.mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
  info.prg_banks    = header[4];
  info.chr_banks    = header[5];
  info.chr_ram      = (info.chr_banks == 0);
  info.prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  info.mirroring    = (header[6] & 1) ? Vertical : Horizontal;

  LOG(lib::log::Info) << "16KB PRG-ROM banks: " << info.prg_banks;
  LOG(lib::log::Info) << "8KB CHR-ROM banks: " << info.chr_banks;
  LOG(lib::log::Info) << "Name table mirroring: " << +(header[6] & 0xB);
  LOG(lib::log::Info) << "Mirroring: "
                      << (info.mirroring ? "Vertical" : "Horizontal");
  LOG(lib::log::Info) << "Mapper #: " << info.mapper_num;
  LOG(lib::log::Info) << "PRG RAM size: " << info.prg_ram_size;

  std::vector<uint8_t> prg(info.prg_banks * ct::prg_bank_size);
  std::vector<uint8_t> chr(info.chr_banks * ct::chr_bank_size);

  rom.read(reinterpret_cast<char*>(prg.data()), prg.size());
  rom.read(reinterpret_cast<char*>(chr.data()), chr.size());

  rom.close();

  using std::move;

  switch (info.mapper_num) {
    case 0:
      mapper = std::make_unique<mapper0>(this->info, move(prg), move(chr));
      break;
    case 1:
      mapper = std::make_unique<mapper1>(this->info, move(prg), move(chr));
      break;
    case 2:
      mapper = std::make_unique<mapper2>(this->info, move(prg), move(chr));
      break;
    case 4:
      mapper = std::make_unique<mapper4>(this->info, move(prg), move(chr));
      break;
    default: throw std::runtime_error("Mapper not implemented");
  }

  mapper->set_bus(*this->bus);
  mapper->reset();

  std::ifstream prg_ram_file(
      std::filesystem::path(rom_path).replace_extension(".srm"),
      std::ios::binary);

  if (prg_ram_file) {
    std::vector<uint8_t> prg_ram(info.prg_ram_size, 0);
    prg_ram_file.read(reinterpret_cast<char*>(prg_ram.data()), prg_ram.size());
    this->mapper->set_prg_ram(move(prg_ram));
  }
}

void cartridge::dump_prg_ram() const
{
  auto prg_ram = this->mapper->get_prg_ram();

  std::ofstream prg_ram_file(
      std::filesystem::path(this->info.path).replace_extension(".srm"),
      std::ios::binary);

  // std::ofstream ofp{prg_ram_file, std::ios::out | std::ios::binary};
  prg_ram_file.write(
      reinterpret_cast<const char*>(prg_ram.data()), prg_ram.size());
}

uint8_t cartridge::prg_read(uint16_t addr) const
{
  return mapper->prg_read(addr);
}

void cartridge::prg_write(uint16_t addr, uint8_t value)
{
  mapper->prg_write(addr, value);
}

uint8_t cartridge::chr_read(uint16_t addr) const
{
  return mapper->chr_read(addr);
}

void cartridge::chr_write(uint16_t addr, uint8_t value)
{
  mapper->chr_write(addr, value);
}

void cartridge::scanline_counter()
{
  mapper->scanline_counter();
}
}  // namespace nes
