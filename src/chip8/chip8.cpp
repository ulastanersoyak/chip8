#include "chip8.hpp"
#include "default_font.hpp"
#include "display.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>

chip8::chip8() noexcept
    : mem{std::make_unique<std::array<std::uint8_t, MEM_SIZE>>()},
      disp{std::make_unique<display>()}, instr_ptr{0}, idx_reg{0}, stack_ptr{0},
      delay_timer{0}, sound_timer{0},
      gpr{std::make_unique<std::array<std::uint8_t, 16>>()} {

  for (std::size_t i = 0; i < default_font.size(); i++) {
    std::copy_n(default_font[i].begin(), default_font[i].size(),
                mem->begin() + 0x50 + (i * default_font[i].size()));
  }
}

[[nodiscard]] instr chip8::fetch() noexcept {
  // TODO: error handling for out of bounds mem access
  const auto full_instr =
      static_cast<std::uint16_t>((*mem)[instr_ptr + 1] << 8 |
                                 static_cast<std::uint16_t>((*mem)[instr_ptr]));
  const auto X = static_cast<uint8_t>((full_instr >> 8) & 0x0F);
  const auto Y = static_cast<uint8_t>((full_instr >> 4) & 0x00F);
  const auto N = static_cast<uint8_t>((full_instr) & 0x000F);
  return instr{.X = X,
               .Y = Y,
               .N = N,
               .NN = static_cast<std::uint8_t>(((Y << 4) | N)),
               .NNN = static_cast<uint16_t>((X << 8) | (Y << 4) | N)};
}
