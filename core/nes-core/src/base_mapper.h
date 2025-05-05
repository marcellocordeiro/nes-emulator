#pragma once

#include <array>
#include <memory>

#include "lib/common.h"
#include "types/ppu_types.h"
#include "utility/snapshotable.hpp"

namespace nes {
class BaseMapper : public utility::Snapshotable {
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

  auto get_prg_addr(u16 addr) const -> usize;
  auto get_chr_addr(u16 addr) const -> usize;

  virtual void write(u16, u8);

  template <usize>
  void set_prg_map(usize, i32);

  template <usize>
  void set_chr_map(usize, i32);

  virtual void scanline_counter();

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

  // TODO: save mirroring
  // TODO: fix this
  std::shared_ptr<bool> irq_conn;
  std::shared_ptr<MirroringType> mirroring_conn;

  usize prg_size = 0;
  usize chr_size = 0;

private:
  std::array<usize, 4> prg_map = {};
  std::array<usize, 8> chr_map = {};
};
} // namespace nes
