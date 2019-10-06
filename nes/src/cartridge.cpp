#include "nes/cartridge.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <log.h>
#include <nes/cpu.h>
#include <nes/emulator.h>
#include <nes/mappers/mapper0.h>
#include <nes/mappers/mapper1.h>
#include <nes/mappers/mapper2.h>
#include <nes/mappers/mapper4.h>
#include <nes/mappers/mapper7.h>
#include <nes/ppu.h>
#include <nes/utility/file_manager.h>
#include <nes/utility/ips_patch.h>

namespace nes {
cartridge::cartridge(emulator& emu_ref) : emu(emu_ref) {}
cartridge::~cartridge() = default;

base_mapper* cartridge::get_mapper()
{
  return mapper.get();
}

void cartridge::load()
{
  using namespace mirroring;

  auto rom = util::fmngr.get_rom();

  // todo
  // Patch the ROM here
  if (util::fmngr.has_patch()) {
    rom = util::ips_patch(util::fmngr.get_patch_path()).patch(rom);
  }

  std::array<uint8_t, 16> header;
  std::copy(rom.begin(), rom.begin() + 16, header.begin());

  // info.path         = rom_path;
  info.rom_size     = rom.size();
  info.mapper_num   = (header[7] & 0xF0) | (header[6] >> 4);
  info.prg_size     = header[4] * ct::prg_bank_size;
  info.chr_size     = header[5] * ct::chr_bank_size;
  info.chr_ram      = (info.chr_size == 0);
  info.prg_ram_size = header[8] ? header[8] * 0x2000 : 0x2000;
  info.mirroring    = (header[6] & 1) ? Vertical : Horizontal;

  switch (info.mapper_num) {
    case 0: mapper = std::make_unique<mapper0>(*this); break;
    case 1: mapper = std::make_unique<mapper1>(*this); break;
    case 2: mapper = std::make_unique<mapper2>(*this); break;
    case 4: mapper = std::make_unique<mapper4>(*this); break;
    case 7: mapper = std::make_unique<mapper7>(*this); break;
    default:
      throw std::runtime_error(
          fmt::format("Mapper #{} not implemented", info.mapper_num));
  }

  LOG(Info, "PRG-ROM size (16KB banks): {}", info.prg_size);
  LOG(Info, "CHR-ROM size (8KB banks): {}", info.chr_size);
  LOG(Info, "Mirroring: {}", (info.mirroring ? "Vertical" : "Horizontal"));
  LOG(Info, "Mapper #: {}", info.mapper_num);
  LOG(Info, "PRG RAM size: {}", info.prg_ram_size);

  auto prg_start = 16;
  auto prg_end   = 16 + info.prg_size;
  auto chr_start = prg_end;
  auto chr_end   = prg_end + info.chr_size;

  std::vector prg(rom.begin() + prg_start, rom.begin() + prg_end);
  std::vector chr(rom.begin() + chr_start, rom.begin() + chr_end);
  std::vector prg_ram(info.prg_ram_size, uint8_t{});

  if (info.chr_ram) {
    chr.resize(0x2000);
  }

  if (util::fmngr.has_prg_ram()) {
    std::ifstream prg_ram_file(
        util::fmngr.get_prg_ram_path(), std::ios::binary);
    prg_ram_file.read(reinterpret_cast<char*>(prg_ram.data()), prg_ram.size());
  }

  mapper->set_prg_rom(std::move(prg));
  mapper->set_chr_rom(std::move(chr));
  mapper->set_prg_ram(std::move(prg_ram));
  mapper->reset();

  set_mirroring(info.mirroring);
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
  if (info.chr_ram) {
    mapper->chr_write(addr, value);
  } else {
    LOG(Error, "This cartridge doesn't have CHR-RAM");
    throw std::runtime_error("This cartridge doesn't have CHR-RAM");
  }
}

void cartridge::scanline_counter()
{
  mapper->scanline_counter();
}

void cartridge::set_mirroring(int mode)
{
  emu.get_ppu()->set_mirroring(mode);
}

void cartridge::set_cpu_irq(bool value)
{
  emu.get_cpu()->set_irq(value);
}

void cartridge::dump_prg_ram() const
{
  const auto& prg_ram = mapper->get_prg_ram();

  std::ofstream prg_ram_file(util::fmngr.get_prg_ram_path(), std::ios::binary);
  prg_ram_file.write(
      reinterpret_cast<const char*>(prg_ram.data()), prg_ram.size());
}
}  // namespace nes