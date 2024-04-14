#pragma once

#include "../display/display.hpp"
#include "../instr/instr.hpp"
#include "config.hpp"

#include <array>
#include <cstdint>
#include <memory>

class chip8 final {
  std::unique_ptr<std::array<std::uint8_t, MEM_SIZE>> mem;
  std::unique_ptr<display> disp;
  std::unique_ptr<std::array<std::uint8_t, REGISTER_COUNT>> gpr;
  std::uint16_t instr_ptr;
  std::uint16_t idx_reg;
  std::uint16_t stack_ptr;
  std::uint8_t delay_timer;
  std::uint8_t sound_timer;

  [[nodiscard]] instr decode(uint16_t opcode) noexcept;

public:
  chip8() noexcept;
  [[nodiscard]] display *get_display() const noexcept;
  [[nodiscard]] instr fetch() noexcept;
};
