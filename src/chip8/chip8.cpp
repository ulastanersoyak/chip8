#include "chip8.hpp"
#include "config.hpp"
#include "default_font.hpp"
#include "display.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>

chip8::chip8() noexcept
    : mem{std::make_unique<std::array<std::uint8_t, MEM_SIZE>>()},
      disp{std::make_unique<display>()},
      gpr{std::make_unique<std::array<std::uint8_t, REGISTER_COUNT>>()},
      instr_ptr{0}, idx_reg{0}, stack_ptr{0}, delay_timer{0}, sound_timer{0} {
  for (std::size_t i = 0; i < default_font.size(); i++) {
    std::copy_n(default_font.at(i).begin(), default_font.at(i).size(),
                mem->begin() + FONT_MEMORY_OFFSET +
                    (i * default_font.at(i).size()));
  }
}

[[nodiscard]] instr chip8::fetch() noexcept {
  return chip8::decode(static_cast<std::uint16_t>(
      (*mem).at(instr_ptr + 1) << 8 |
      static_cast<std::uint16_t>((*mem).at(instr_ptr))));
}

[[nodiscard]] instr chip8::decode(uint16_t opcode) noexcept {
  const auto first_nibble = static_cast<std::uint8_t>((opcode >> 12) & 0x0F);
  const auto second_nibble = static_cast<std::uint8_t>((opcode >> 8) & 0x0F);
  const auto third_nibble = static_cast<std::uint8_t>((opcode >> 4) & 0x00F);
  const auto fourth_nibble = static_cast<std::uint8_t>((opcode) & 0x000F);
  return instr{.first_nibble = first_nibble,
               .second_nibble = second_nibble,
               .third_nibble = third_nibble,
               .fourth_nibble = fourth_nibble,
               .second_byte = static_cast<std::uint8_t>(
                   ((third_nibble << 4) | fourth_nibble)),
               .except_first_nibble = static_cast<uint16_t>(
                   (second_nibble << 8) | (third_nibble << 4) | fourth_nibble)};
}

[[nodiscard]] display *chip8::get_display() const noexcept {
  return this->disp.get();
}

void chip8::jump(std::uint16_t addr) noexcept { this->instr_ptr = addr; }

void chip8::set_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept {
  (*this->gpr).at(reg_idx) = val;
}

void chip8::add_to_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept {
  (*this->gpr).at(reg_idx) += val;
}

void chip8::set_idx_reg(std::uint16_t val) noexcept { this->idx_reg = val; }
