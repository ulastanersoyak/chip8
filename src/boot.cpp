#include "chip8.hpp"
#include <SDL2/SDL_events.h>
#include <print>

// TODO: timer & FX07 FX15 FX18
// TODO: keypad & EX9E EXA1

// 0x6109

int
main ([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  if (argc != 2)
    {
      std::println ("usage: {} <rom_name>", argv[0]);
      return EXIT_FAILURE;
    }
  chip8 chip{};
  std::string_view rom_name = argv[1];
  if (!chip.read_rom (rom_name))
    {
      std::println ("failed to open the file: {}.ch8", rom_name);
      return EXIT_FAILURE;
    }
  bool quit = false;
  SDL_Event event;
  while (!quit)
    {
      auto inst = chip.fetch ();
      chip.execute (inst);
      SDL_PollEvent (&event);
      if (event.type == SDL_QUIT)
        {
          quit = true;
        }
      chip.get_display ()->refresh_screen ();
    }
  return EXIT_SUCCESS;
}
