#pragma once

#include <array>
#include <memory>

#include <common.h>
#include "Types/PPU_Types.h"
#include "Utility/Snapshotable.hpp"

namespace nes {
class BaseMapper : public Utility::Snapshotable {
public:
  BaseMapper() = default;
  ~BaseMapper() override = default;

  BaseMapper(const BaseMapper&) = delete;
  BaseMapper(BaseMapper&&) = delete;
  auto operator=(const BaseMapper&) -> BaseMapper& = delete;
  auto operator=(BaseMapper&&) -> BaseMapper& = delete;

  using MirroringType = types::ppu::MirroringType;

  virtual void reset() = 0;

  void set_mirroring(MirroringType);
  void set_irq(bool);

  auto get_prg_addr(uint16_t addr) const -> size_t;
  auto get_chr_addr(uint16_t addr) const -> size_t;

  virtual void write(uint16_t, uint8_t);

  template <size_t>
  void set_prg_map(size_t, int);

  template <size_t>
  void set_chr_map(size_t, int);

  virtual void scanline_counter();

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

  // TODO: save mirroring
  // TODO: fix this
  std::shared_ptr<bool> irq_conn;
  std::shared_ptr<MirroringType> mirroring_conn;

  size_t prg_size = 0;
  size_t chr_size = 0;

private:
  std::array<size_t, 4> prg_map = {};
  std::array<size_t, 8> chr_map = {};
};
}  // namespace nes
