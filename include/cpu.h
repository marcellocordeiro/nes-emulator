#pragma once

#include <array>

#include "common.h"
#include "types/cpu.h"
#include "types/forward_decl.h"

namespace nes {
class cpu {
public:
  cpu(nes::ppu&, nes::apu&, nes::cartridge&, nes::controller&);

  void power_on();
  void reset();

  void dma_oam(uint8_t);
  void set_nmi(bool = true);
  void set_irq(bool = true);

  int dmc_reader(uint16_t);

  void run_frame();

  // friend class debugger;

private:
  nes::ppu& ppu;
  nes::apu& apu;
  nes::cartridge& cartridge;
  nes::controller& controller;

  nes::types::cpu::state     state;
  std::array<uint8_t, 0x800> ram = {};

  void tick();

  uint8_t read(uint16_t) const;
  void    write(uint16_t, uint8_t);

  uint8_t memory_read(uint16_t);
  void    memory_write(uint16_t, uint8_t);

  uint8_t peek(uint16_t) const;

  uint16_t peek_imm() const;
  uint16_t peek_rel() const;
  uint16_t peek_zp() const;
  uint16_t peek_zpx() const;
  uint16_t peek_zpy() const;
  uint16_t peek_ab() const;
  uint16_t peek_abx() const;
  uint16_t peek_aby() const;
  uint16_t peek_ind() const;
  uint16_t peek_indx() const;
  uint16_t peek_indy() const;

  int elapsed() const;

  const int total_cycles     = 29781;
  int       remaining_cycles = 0;

  //
  // All functions defined here are
  // implemented in cpu_instructions.cpp
  //

  void execute();

  // Instructions

  //
  // Auxiliary
  //

  template <auto Mode> uint16_t get_operand();

  void    add(uint8_t);
  uint8_t shift_left(uint8_t);   // Arithmetic left shift
  uint8_t shift_right(uint8_t);  // Logical right shift
  uint8_t rotate_left(uint8_t);
  uint8_t rotate_right(uint8_t);

  void compare(uint8_t, uint8_t);

  void branch(bool);

  void    push(uint8_t);
  uint8_t pop();

  bool crosses_page(uint16_t, uint8_t) const;
  bool crosses_page(uint16_t, int8_t) const;

  //
  // Storage
  //

  template <auto Mode> void LDA();
  template <auto Mode> void LDX();
  template <auto Mode> void LDY();

  template <auto Mode> void STA();
  template <auto Mode> void STX();
  template <auto Mode> void STY();

  void TAX();
  void TAY();
  void TSX();
  void TXA();
  void TXS();
  void TYA();

  //
  // Math
  //

  template <auto Mode> void ADC();
  template <auto Mode> void SBC();
  template <auto Mode> void INC();
  template <auto Mode> void DEC();
  void                      INX();
  void                      INY();
  void                      DEX();
  void                      DEY();

  //
  // Bitwise
  //

  template <auto Mode> void AND();
  template <auto Mode> void ORA();
  template <auto Mode> void EOR();
  template <auto Mode> void LSR();
  template <auto Mode> void ASL();
  template <auto Mode> void ROL();
  template <auto Mode> void ROR();

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

  template <auto Mode> void CMP();
  template <auto Mode> void CPX();
  template <auto Mode> void CPY();
  template <auto Mode> void BIT();

  //
  // Jumps and branches
  //

  template <auto Mode> void JMP();
  void                      JSR();
  void                      RTS();
  void                      RTI();

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

  template <auto Mode> void NOP();
  template <auto Mode> void LAX();  // LDA then TXA
  template <auto Mode> void SAX();  // A & X
  template <auto Mode> void DCP();  // DEC then CMP
  template <auto Mode> void ISB();  // INC then SBC
  template <auto Mode> void SLO();  // ASL then ORA
  template <auto Mode> void RLA();  // ROL then AND
  template <auto Mode> void SRE();  // LSR then EOR
  template <auto Mode> void RRA();  // ROR then ADC
};
}  // namespace nes
