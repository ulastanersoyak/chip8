#pragma once

#include "config.hpp"
#include <array>
#include <cstdint>
#include <memory>

struct chip8 {
  std::unique_ptr<std::array<std::uint8_t, MEM_SIZE>> mem;
  std::unique_ptr<std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>
      display;
  std::uint16_t instr_ptr;
  std::uint16_t idx_reg;
  std::uint16_t stack_ptr;
  std::uint8_t delay_timer;
  std::uint8_t sound_timer;
  // last register is used as flag
  std::unique_ptr<std::array<std::uint8_t, 16>> gpr;

public:
  chip8();
};
