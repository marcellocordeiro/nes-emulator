#pragma once

#include <array>
#include <vector>

#include "common.h"
#include "types/cartridge.h"
#include "types/forward_decl.h"
#include "utility/snapshotable.h"

namespace nes {
class mapper : public util::snapshotable {
public:
  mapper(nes::cartridge&);
  virtual ~mapper() = default;

  virtual void reset() = 0;

  void set_prg_rom(std::vector<uint8_t>&&);
  void set_chr_rom(std::vector<uint8_t>&&);

  void                 set_prg_ram(std::vector<uint8_t>&&);
  std::vector<uint8_t> get_prg_ram() const;

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  virtual void prg_write(uint16_t, uint8_t);
  virtual void chr_write(uint16_t, uint8_t);

  template <size_t> void set_prg_map(int, int);
  template <size_t> void set_chr_map(int, int);

  virtual void scanline_counter();

  virtual void save(std::ofstream&) override;
  virtual void load(std::ifstream&) override;

protected:
  nes::cartridge& cartridge;

  std::vector<uint8_t> prg;
  std::vector<uint8_t> prg_ram;
  std::vector<uint8_t> chr;

  std::array<size_t, 4> prg_map{};
  std::array<size_t, 8> chr_map{};
};
}  // namespace nes
