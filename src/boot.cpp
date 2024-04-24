#include "chip8.hpp"
#include "config.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// TODO: 32 byte stack (16/2)
// TODO: timer
// TODO: keypad
// TODO: DXYN

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <rom_name>\n";
    return EXIT_FAILURE;
  }
  std::string_view rom_name = argv[1];
  chip8 chip{};
  if (!chip.read_rom(rom_name)) {
    return EXIT_FAILURE;
  }
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    auto inst = chip.fetch();
    if (inst.first_nibble == 0x0) {
      chip.get_display()->clear_window();
    } else if (inst.first_nibble == 0x1) {
      chip.jump(inst.except_first_nibble);
    } else if (inst.first_nibble == 0x6) {
      chip.set_gpr(inst.second_nibble, inst.second_byte);
    } else if (inst.first_nibble == 0x7) {
      chip.add_to_gpr(inst.second_nibble, inst.second_byte);
    } else if (inst.first_nibble == 0xA) {
      chip.set_idx_reg(inst.except_first_nibble);
    } else if (inst.first_nibble == 0xD) {
      pos pos = {.x = static_cast<uint16_t>(chip.get_gpr(inst.second_nibble) %
                                            DISPLAY_X),
                 .y = static_cast<uint16_t>(chip.get_gpr(inst.third_nibble) %
                                            DISPLAY_Y)};
      chip.draw(pos, inst.fourth_nibble);
    }
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      quit = true;
    }
    chip.get_display()->refresh_screen();
  }
  return 0;
}
