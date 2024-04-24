#include "chip8.hpp"
#include <iostream>

// TODO: 32 byte stack (16/2)
// TODO: timer
// TODO: keypad
// TODO: remove 1 millisecond delay

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
    chip.execute(inst);
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      quit = true;
    }
    chip.get_display()->refresh_screen();
  }
  return 0;
}
