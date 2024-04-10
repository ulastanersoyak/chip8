#include "chip8.hpp"
#include "default_font.hpp"
#include "display.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>

chip8::chip8() noexcept
    : mem{std::make_unique<std::array<std::uint8_t, MEM_SIZE>>()},
      disp{std::make_unique<display>()},
      gpr{std::make_unique<std::array<std::uint8_t, 16>>()}, instr_ptr{0},
      idx_reg{0}, stack_ptr{0}, delay_timer{0}, sound_timer{0} {
  for (std::size_t i = 0; i < default_font.size(); i++) {
    std::copy_n(default_font[i].begin(), default_font[i].size(),
                mem->begin() + 0x50 + (i * default_font[i].size()));
  }
}

[[nodiscard]] instr chip8::fetch() noexcept {
  return this->decode(static_cast<std::uint16_t>(
      (*mem)[instr_ptr + 1] << 8 |
      static_cast<std::uint16_t>((*mem)[instr_ptr])));
}

[[nodiscard]] instr chip8::decode(uint16_t opcode) noexcept {
  const auto X = static_cast<uint8_t>((opcode >> 8) & 0x0F);
  const auto Y = static_cast<uint8_t>((opcode >> 4) & 0x00F);
  const auto N = static_cast<uint8_t>((opcode) & 0x000F);
  return instr{.X = X,
               .Y = Y,
               .N = N,
               .NN = static_cast<std::uint8_t>(((Y << 4) | N)),
               .NNN = static_cast<uint16_t>((X << 8) | (Y << 4) | N)};
}

[[nodiscard]] display *chip8::get_display() const noexcept {
  return this->disp.get();
}
