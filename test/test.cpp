#include "chip8.hpp"

#include <SDL2/SDL_events.h>

#include <filesystem>
#include <print>
#include <vector>

int
main ()
{
  std::vector<std::string> test_roms{};
  std::filesystem::path current_path
      = std::filesystem::current_path () / "../../test/";
  try
    {
      for (const auto &entry :
           std::filesystem::directory_iterator (current_path))
        {
          if (entry.is_regular_file () && entry.path ().extension () == ".ch8")
            {
              test_roms.push_back (
                  entry.path ().filename ().stem ().string ());
            }
        }
    }
  catch (const std::filesystem::filesystem_error &error)
    {
      std::print ("error while loading test roms\n{}", error.what ());
    }
  for (const auto &rom_name : test_roms)
    {

      chip8 chip{};
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
          if (event.type == SDL_KEYDOWN)
            {
              if (event.key.keysym.sym == SDLK_RETURN)
                {
                  quit = true;
                }
            }
          chip.get_display ()->refresh_screen ();
        }
    }
  return EXIT_SUCCESS;
}
