#include "chip8.hpp"
#include "default_font.hpp"

chip8::chip8()
    : mem(std::make_unique<std::array<std::uint8_t, MEM_SIZE>>()),
      display(std::make_unique<
              std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>()),
      instr_ptr(0), idx_reg(0), stack_ptr(0), delay_timer(0), sound_timer(0),
      gpr(std::make_unique<std::array<std::uint8_t, 16>>()) {

  for (std::size_t i = 0; i < default_font.size(); i++) {
    std::copy_n(default_font[i].begin(), default_font[i].size(),
                mem->begin() + 0x50 + (i * default_font[i].size()));
  }
}
