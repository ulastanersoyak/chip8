#include "chip8.hpp"
#include <iostream>

// TODO: timer & FX07 FX15 FX18
// TODO: keypad & EX9E EXA1

// 0x6109

int
main ([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  if (argc != 2)
    {
      std::cout << "usage: " << argv[0] << " <rom_name>\n";
      return EXIT_FAILURE;
    }
  chip8 chip{};
  std::string_view rom_name = argv[1];
  if (!chip.read_rom (rom_name))
    {
      std::cout << "failed to open the file: " << rom_name << ".ch8\n";
      return EXIT_FAILURE;
    }
  bool quit = false;
  SDL_Event event;
  while (!quit)
    {
      auto inst = chip.fetch ();
      chip.execute (inst);
      SDL_WaitEvent (&event);
      if (event.type == SDL_QUIT)
        {
          quit = true;
        }
      chip.get_display ()->refresh_screen ();
    }
  return 0;
}
