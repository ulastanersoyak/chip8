#include "chip8.hpp"

// TODO: 32 byte stack (16/2)
// TODO: timer
// TODO: keypad
// TODO: DXYN

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  chip8 chip{};
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    auto inst = chip.fetch();
    if (inst.third_nibble == 0xE) {
      chip.get_display()->clear_window();
    } else if (inst.first_nibble == 0x1) {
      chip.jump(inst.except_first_nibble);
    } else if (inst.first_nibble == 0x6) {
      chip.set_gpr(inst.second_nibble, inst.second_byte);
    } else if (inst.first_nibble == 0x7) {
      chip.add_to_gpr(inst.second_nibble, inst.second_byte);
    } else if (inst.first_nibble == 0xA) {
      chip.set_idx_reg(inst.except_first_nibble);
    }

    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      quit = true;
    }
  }
  return 0;
}
