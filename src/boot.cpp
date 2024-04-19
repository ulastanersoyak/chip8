#include "chip8.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>

int main() {
  chip8 chip{};

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return EXIT_FAILURE;
  }

  SDL_SetRenderDrawColor(chip.get_display()->get_renderer(), 255, 255, 255,
                         255);

  SDL_RenderClear(chip.get_display()->get_renderer());
  SDL_RenderPresent(chip.get_display()->get_renderer());

  SDL_RenderClear(chip.get_display()->get_renderer());
  for (std::uint16_t i = 0; i < 5; i++) {
    chip.get_display()->flip_pixel({i, 0});
  }
  chip.get_display()->flip_pixel({0, 0});
  SDL_Delay(10000);

  SDL_Quit();
  return 0;
}
