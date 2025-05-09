#pragma once

#include <array>
#include <memory>

#include "lib/common.hpp"
#include "types/ppu_types.hpp"
#include "utility/snapshotable.hpp"

namespace nes {
class BaseMapper : public utility::Snapshotable {
public:
  using MirroringType = types::ppu::MirroringType;

  virtual void reset() = 0;

  void set_mirroring(MirroringType);
  void set_irq(bool);

  [[nodiscard]] auto get_prg_addr(u16 addr) const -> usize;
  [[nodiscard]] auto get_chr_addr(u16 addr) const -> usize;

  virtual void write(u16, u8);

  template <usize>
  void set_prg_map(usize, i32);

  template <usize>
  void set_chr_map(usize, i32);

  virtual void scanline_counter();

  void save(std::ofstream& out) const override;
  void load(std::ifstream& in) override;

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
