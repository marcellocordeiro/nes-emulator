#pragma once

#include <array>
#include <memory>

#include "lib/common.hpp"
#include "types/ppu_types.hpp"

namespace nes {
class BaseMapper {
public:
  using MirroringType = types::ppu::MirroringType;

  BaseMapper() = default;
  virtual ~BaseMapper() = default;

  BaseMapper(const BaseMapper&) = delete;
  auto operator=(const BaseMapper&) -> BaseMapper& = delete;
  BaseMapper(const BaseMapper&&) = delete;
  auto operator=(BaseMapper&&) -> BaseMapper& = delete;

  virtual void reset() = 0;

  [[nodiscard]] auto get_mirroring() const -> MirroringType;
  void set_mirroring(MirroringType value);
  void set_irq(bool value) const;

  [[nodiscard]] auto get_prg_addr(u16 addr) const -> usize;
  [[nodiscard]] auto get_chr_addr(u16 addr) const -> usize;

  virtual void write(u16 addr, u8 value);

  template <std::size_t Size>
  void set_prg_map(usize slot, i32 page);

  template <std::size_t Size>
  void set_chr_map(usize slot, usize page);

  virtual void increment_scanline_counter();

  // TODO: fix this
  std::shared_ptr<bool> irq;

  usize prg_size = 0;
  usize chr_size = 0;

private:
  MirroringType mirroring = MirroringType::Unknown;

  std::array<usize, 4> prg_map = {};
  std::array<usize, 8> chr_map = {};
};
} // namespace nes
