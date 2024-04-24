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

  [[nodiscard]] static instr decode(uint16_t opcode) noexcept;

public:
  chip8() noexcept;
  [[nodiscard]] bool read_rom(std::string_view rom_name);
  [[nodiscard]] display *get_display() const noexcept;
  [[nodiscard]] std::uint8_t get_gpr(std::uint8_t idx) const noexcept;
  [[nodiscard]] instr fetch() noexcept;
  void jump(std::uint16_t addr) noexcept;
  void set_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept;
  void add_to_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept;
  void set_idx_reg(std::uint16_t val) noexcept;
  void draw(const pos &starting_position, std::uint8_t size) noexcept;
};
