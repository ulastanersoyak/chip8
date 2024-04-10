#include "chip8.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>

int main() {
  chip8 chip{};

  // Initialize SDL.
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return EXIT_FAILURE;
  }

  SDL_Quit();
  return 0;
}
