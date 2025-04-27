#pragma once

#include <array>
#include <memory>

#include <lib/common.h>
#include "types/cpu_types.h"
#include "utility/snapshotable.hpp"

namespace nes {
class Cpu final : public utility::Snapshotable {
public:
  Cpu(const Cpu&) = delete;
  Cpu(Cpu&&) = delete;
  auto operator=(const Cpu&) -> Cpu& = delete;
  auto operator=(Cpu&&) -> Cpu& = delete;

  using ram_type = std::array<uint8_t, 0x800>;

  static auto get() -> Cpu&;

  void power_on();
  void reset();

  void dma_oam(uint8_t);

  void run_frame();

  std::shared_ptr<bool> irq_conn;
  std::shared_ptr<bool> nmi_conn;

  //
  // Read without side effects
  //

  auto get_state() const -> types::cpu::State;

  auto peek(uint16_t) const -> uint8_t;

  auto peek_imm() const -> uint16_t;
  auto peek_rel() const -> uint16_t;
  auto peek_zp() const -> uint16_t;
  auto peek_zpx() const -> uint16_t;
  auto peek_zpy() const -> uint16_t;
  auto peek_ab() const -> uint16_t;
  auto peek_abx() const -> uint16_t;
  auto peek_aby() const -> uint16_t;
  auto peek_ind() const -> uint16_t;
  auto peek_indx() const -> uint16_t;
  auto peek_indy() const -> uint16_t;

  //
  // Snapshot
  //

  void save(std::ofstream&) const override;
  void load(std::ifstream&) override;

private:
  Cpu() = default;

  types::cpu::State state;
  ram_type ram = {};

  void tick();

  auto read(uint16_t) const -> uint8_t;
  void write(uint16_t, uint8_t);

  auto memory_read(uint16_t) -> uint8_t;
  void memory_write(uint16_t, uint8_t);

  //
  // All functions defined here are
  // implemented in cpu_instructions.cpp
  //

  void execute();

  // Instructions

  //
  // Auxiliary
  //

  void add(uint8_t);
  auto shift_left(uint8_t) -> uint8_t;  // Arithmetic left shift
  auto shift_right(uint8_t) -> uint8_t; // Logical right shift
  auto rotate_left(uint8_t) -> uint8_t;
  auto rotate_right(uint8_t) -> uint8_t;

  void compare(uint8_t, uint8_t);

  void push(uint8_t);
  auto pop() -> uint8_t;

  void branch(bool);

  auto crossed_page(uint16_t, uint16_t) const -> bool;

  template <auto Mode>
  auto get_operand() -> uint16_t;

  //
  // Storage
  //

  template <auto Mode>
  void LDA();

  template <auto Mode>
  void LDX();

  template <auto Mode>
  void LDY();

  template <auto Mode>
  void STA();

  template <auto Mode>
  void STX();

  template <auto Mode>
  void STY();

  void TAX();
  void TAY();
  void TSX();
  void TXA();
  void TXS();
  void TYA();

  //
  // Math
  //

  template <auto Mode>
  void ADC();

  template <auto Mode>
  void SBC();

  template <auto Mode>
  void INC();

  template <auto Mode>
  void DEC();

  void INX();
  void INY();
  void DEX();
  void DEY();

  //
  // Bitwise
  //

  template <auto Mode>
  void AND();

  template <auto Mode>
  void ORA();

  template <auto Mode>
  void EOR();

  template <auto Mode>
  void LSR();

  template <auto Mode>
  void ASL();

  template <auto Mode>
  void ROL();

  template <auto Mode>
  void ROR();

  //
  // Flags
  //

  void CLC();
  void CLD();
  void CLI();
  void CLV();
  void SEC();
  void SED();
  void SEI();

  template <auto Mode>
  void CMP();

  template <auto Mode>
  void CPX();

  template <auto Mode>
  void CPY();

  template <auto Mode>
  void BIT();

  //
  // Jumps and branches
  //

  template <auto Mode>
  void JMP();

  void JSR();
  void RTS();
  void RTI();

  void BCC();
  void BCS();
  void BEQ();
  void BMI();
  void BNE();
  void BPL();
  void BVC();
  void BVS();

  //
  // Stack
  //

  void PHA();
  void PLA();
  void PHP();
  void PLP();

  //
  // System
  //

  void INT_NMI();
  void INT_RST();
  void INT_IRQ();
  void INT_BRK();

  void NOP();

  //
  // Unofficial instructions
  //

  template <auto Mode>
  void NOP();

  template <auto Mode>
  void LAX();

  template <auto Mode>
  void SAX();

  template <auto Mode>
  void DCP();

  template <auto Mode>
  void ISB();

  template <auto Mode>
  void SLO();

  template <auto Mode>
  void RLA();

  template <auto Mode>
  void SRE();

  template <auto Mode>
  void RRA();

  template <auto Mode>
  void AAC();

  template <auto Mode>
  void ASR();

  template <auto Mode>
  void ARR();

  template <auto Mode>
  void ATX();

  template <auto Mode>
  void AXS();

  template <auto Mode>
  void SYA();

  template <auto Mode>
  void SXA();
};
} // namespace nes
