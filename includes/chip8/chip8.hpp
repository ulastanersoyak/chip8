#pragma once

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

  void jump (std::uint16_t addr) noexcept;
  void call_subroutine (std::uint16_t addr) noexcept;
  void ret_subroutine () noexcept;
  void set_gpr (std::uint8_t reg_idx, std::uint8_t val) noexcept;
  void add_to_gpr (std::uint8_t reg_idx, std::uint8_t val) noexcept;
  void set_idx_reg (std::uint16_t val) noexcept;
  void draw (const pos &starting_position, std::uint8_t size) noexcept;
  [[nodiscard]] static std::uint8_t get_random () noexcept;

public:
  [[nodiscard]] static instr decode (uint16_t opcode) noexcept;
  chip8 () noexcept;
  [[nodiscard]] bool read_rom (std::string_view rom_name);
  [[nodiscard]] display *get_display () const noexcept;
  [[nodiscard]] std::uint8_t get_gpr (std::uint8_t idx) const noexcept;
  [[nodiscard]] instr fetch () noexcept;
  void execute (const instr &inst);
};
