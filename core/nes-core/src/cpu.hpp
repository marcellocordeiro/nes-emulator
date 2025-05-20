#pragma once

#include <array>
#include <memory>

#include "lib/common.hpp"
#include "types/cpu_types.hpp"

namespace nes {
class Cpu final {
public:
  using RamType = std::array<u8, 0x800>;

  [[nodiscard]] static auto get() -> Cpu&;

  void power_on();
  void reset();

  void dma_oam(u16 addr);

  void run_frame();

  std::shared_ptr<bool> irq_conn;
  std::shared_ptr<bool> nmi_conn;

  //
  // Read without side effects
  //

  [[nodiscard]] auto get_state() const -> types::cpu::State;

  [[nodiscard]] auto peek(u16 addr) const -> u8;

  [[nodiscard]] auto peek_imm() const -> u16;
  [[nodiscard]] auto peek_rel() const -> u16;
  [[nodiscard]] auto peek_zp() const -> u16;
  [[nodiscard]] auto peek_zpx() const -> u16;
  [[nodiscard]] auto peek_zpy() const -> u16;
  [[nodiscard]] auto peek_ab() const -> u16;
  [[nodiscard]] auto peek_abx() const -> u16;
  [[nodiscard]] auto peek_aby() const -> u16;
  [[nodiscard]] auto peek_ind() const -> u16;
  [[nodiscard]] auto peek_indx() const -> u16;
  [[nodiscard]] auto peek_indy() const -> u16;

private:
  Cpu() = default;

  types::cpu::State state;
  RamType ram = {};

  void tick();

  [[nodiscard]] auto read(u16 addr) const -> u8;
  void write(u16 addr, u8 value);

  [[nodiscard]] auto memory_read(u16 addr) -> u8;
  void memory_write(u16 addr, u8 value);

  //
  // All functions defined here are
  // implemented in cpu_instructions.cpp
  //

  void execute();

  // Instructions

  //
  // Auxiliary
  //

  void add(u8 value);
  [[nodiscard]] auto shift_left(u8 value) -> u8;  // Arithmetic left shift
  [[nodiscard]] auto shift_right(u8 value) -> u8; // Logical right shift
  [[nodiscard]] auto rotate_left(u8 value) -> u8;
  [[nodiscard]] auto rotate_right(u8 value) -> u8;

  void compare(u8 reg, u8 value);

  void push(u8 value);
  [[nodiscard]] auto pop() -> u8;

  void branch(bool taken);

  [[nodiscard]] static auto will_cross_page(u16 from, u16 to) -> bool;

  template <auto Mode>
  [[nodiscard]] auto get_operand() -> u16;

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
