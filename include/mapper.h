#pragma once

#include <array>
#include <vector>

#include "bus.h"
#include "common.h"
#include "types/cartridge.h"

namespace nes {
class mapper {
public:
  mapper(
      const nes::cartridge_info&,
      std::vector<uint8_t>&&,
      std::vector<uint8_t>&&);
  virtual ~mapper() = default;

  virtual void reset() = 0;

  void set_bus(nes::bus&);

  void                        set_prg_ram(std::vector<uint8_t>&&);
  const std::vector<uint8_t>& get_prg_ram() const;

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  virtual void prg_write(uint16_t, uint8_t);
  virtual void chr_write(uint16_t, uint8_t);

  template <auto> void set_prg_map(int, int);
  template <auto> void set_chr_map(int, int);

  virtual void scanline_counter() {}

protected:
  void set_mirroring(int);

  const nes::cartridge_info& info;

  std::vector<uint8_t> prg;
  std::vector<uint8_t> prg_ram;
  std::vector<uint8_t> chr;

  std::array<size_t, 4> prg_map{};
  std::array<size_t, 8> chr_map{};

  nes::bus* bus = nullptr;
};
}  // namespace nes
