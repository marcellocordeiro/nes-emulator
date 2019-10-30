#pragma once

#include <array>
#include <vector>

#include <common.h>
#include "Utility/Snapshotable.hpp"

namespace nes {
class BaseMapper : public Utility::Snapshotable {
public:
  BaseMapper()          = default;
  virtual ~BaseMapper() = default;

  BaseMapper(const BaseMapper&) = delete;
  BaseMapper(BaseMapper&&)      = delete;
  BaseMapper& operator=(const BaseMapper&) = delete;
  BaseMapper& operator=(BaseMapper&&) = delete;

  virtual void reset() = 0;

  void set_prg_rom(std::vector<uint8_t>&&);
  void set_chr_rom(std::vector<uint8_t>&&);

  void                 set_prg_ram(std::vector<uint8_t>&&);
  std::vector<uint8_t> get_prg_ram() const;

  uint8_t prg_read(uint16_t) const;
  uint8_t chr_read(uint16_t) const;

  virtual void prg_write(uint16_t, uint8_t);
  virtual void chr_write(uint16_t, uint8_t);

  template <size_t> void set_prg_map(size_t, int);
  template <size_t> void set_chr_map(size_t, int);

  virtual void scanline_counter();

  void save(std::ofstream&) override;
  void load(std::ifstream&) override;

protected:
  std::vector<uint8_t> prg;
  std::vector<uint8_t> prg_ram;
  std::vector<uint8_t> chr;

  std::array<size_t, 4> prg_map = {};
  std::array<size_t, 8> chr_map = {};
};
}  // namespace nes
