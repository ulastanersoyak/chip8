#pragma once
#ifndef CHIP8_HPP
#define CHIP8_HPP

#include "../display/display.hpp"
#include "../instr/instr.hpp"
#include "config.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

class chip8 final
{
  std::unique_ptr<std::array<std::uint8_t, MEM_SIZE> > mem;
  std::unique_ptr<display> disp;
  std::unique_ptr<std::array<std::uint8_t, REGISTER_COUNT> > gpr;
  std::unique_ptr<std::vector<std::uint16_t> > stack;
  std::uint16_t instr_ptr;
  std::uint16_t idx_reg;
  std::uint8_t delay_timer;
  std::uint8_t sound_timer;

  // clear the display
  constexpr void inst_00E0 () const noexcept;

  // return from a subroutine
  constexpr void inst_00EE () noexcept;

  // jump to location nnn
  constexpr void inst_1nnn (const instr &inst) noexcept;

  // call subroutine at nnn
  constexpr void inst_2nnn (const instr &inst) noexcept;

  // skip next instruction if Vx = kk
  constexpr void inst_3xkk (const instr &inst) noexcept;

  // skip next instruction if Vx != kk
  constexpr void inst_4xkk (const instr &inst) noexcept;

  // skip next instruction if Vx = Vy
  constexpr void inst_5xy0 (const instr &inst) noexcept;

  // set Vx = kk
  constexpr void inst_6xkk (const instr &inst) noexcept;

  // set Vx = Vx + kk
  constexpr void inst_7xkk (const instr &inst) noexcept;

  // set Vx = Vy
  constexpr void inst_8xy0 (const instr &inst) noexcept;

  // set Vx = Vx OR Vy
  constexpr void inst_8xy1 (const instr &inst) noexcept;

  // set Vx = Vx AND Vy
  constexpr void inst_8xy2 (const instr &inst) noexcept;

  // set Vx = Vx XOR Vy
  constexpr void inst_8xy3 (const instr &inst) noexcept;

  // set Vx = Vx + Vy, set VF = carry
  constexpr void inst_8xy4 (const instr &inst) noexcept;

  // set Vx = Vx - Vy, set VF = NOT borrow
  constexpr void inst_8xy5 (const instr &inst) noexcept;

  // set Vx = Vx SHR 1
  constexpr void inst_8xy6 (const instr &inst) noexcept;

  // set Vx = Vy - Vx, set VF = NOT borrow
  constexpr void inst_8xy7 (const instr &inst) noexcept;

  // set Vx = Vx SHL 1
  constexpr void inst_8xyE (const instr &inst) noexcept;

  // skip next instruction if Vx != Vy
  constexpr void inst_9xy0 (const instr &inst) noexcept;

  // set I = nnn
  constexpr void inst_Annn (const instr &inst) noexcept;

  // jump to location nnn + V0
  constexpr void inst_Bnnn (const instr &inst) noexcept;

  // set Vx = random byte AND kk
  constexpr void inst_Cxkk (const instr &inst) noexcept;

  // display n-byte sprite starting at memory location I at (Vx, Vy), set VF =
  // collision
  constexpr void inst_Dxyn (const instr &inst) noexcept;

  // skip next instruction if key with the value of Vx is pressed
  constexpr void inst_Ex9E (const instr &inst) noexcept;

  // skip next instruction if key with the value of Vx is not pressed
  constexpr void inst_ExA1 (const instr &inst) noexcept;

  // set Vx = delay timer value
  constexpr void inst_Fx07 (const instr &inst) noexcept;

  // wait for a key press, store the value of the key in Vx
  constexpr void inst_Fx0A (const instr &inst) noexcept;

  // set delay timer = Vx
  constexpr void inst_Fx15 (const instr &inst) noexcept;

  // set sound timer = Vx
  constexpr void inst_Fx18 (const instr &inst) noexcept;

  // set I = I + Vx
  constexpr void inst_Fx1E (const instr &inst) noexcept;

  // set I = location of sprite for digit Vx
  constexpr void inst_Fx29 (const instr &inst) noexcept;

  // store BCD representation of Vx in memory locations I, I+1, and I+2
  constexpr void inst_Fx33 (const instr &inst) noexcept;

  // store registers V0 through Vx in memory starting at location I
  constexpr void inst_Fx55 (const instr &inst) noexcept;

  // read registers V0 through Vx from memory starting at location I
  constexpr void inst_Fx65 (const instr &inst) noexcept;

  void set_gpr (std::uint8_t reg_idx, std::uint8_t val) noexcept;
  [[nodiscard]] std::uint8_t get_gpr (std::uint8_t idx) const noexcept;
  [[nodiscard]] constexpr static instr decode (uint16_t opcode) noexcept;
  constexpr void set_idx_reg (std::uint16_t val) noexcept;
  [[nodiscard]] static std::uint8_t get_random () noexcept;

public:
  chip8 () noexcept;
  [[nodiscard]] bool read_rom (std::string_view rom_name);
  [[nodiscard]] display *get_display () const noexcept;
  [[nodiscard]] instr fetch () noexcept;
  void execute (const instr &inst) noexcept;
};

#endif // !CHIP8_HPP
