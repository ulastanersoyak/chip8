#include "chip8.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <iostream>

int main() {
  chip8 chip{};

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return EXIT_FAILURE;
  }

  SDL_SetRenderDrawColor(chip.get_display()->get_renderer(), BLACK);

  SDL_RenderClear(chip.get_display()->get_renderer());
  SDL_RenderPresent(chip.get_display()->get_renderer());

  SDL_RenderClear(chip.get_display()->get_renderer());

  SDL_SetRenderDrawColor(chip.get_display()->get_renderer(), WHITE);
  for (int i = 0; i < 640; i++) {
    SDL_RenderDrawPoint(chip.get_display()->get_renderer(), i, 0);
  }

  SDL_RenderPresent(chip.get_display()->get_renderer());

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
      0, 640, 320, 32,
      SDL_GetWindowPixelFormat(chip.get_display()->get_window()));

  void *x = surface->pixels;
  std::cout << x;

  SDL_Delay(5000);

  SDL_Quit();
  return 0;
}
