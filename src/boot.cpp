#include "chip8.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <cstdlib>

int main() {
  chip8 chip{};

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return EXIT_FAILURE;
  }

  SDL_SetRenderDrawColor(chip.get_display()->get_renderer(), 0, 0, 0, 255);

  SDL_RenderClear(chip.get_display()->get_renderer());
  SDL_RenderPresent(chip.get_display()->get_renderer());

  std::array<pos, 10> arr;
  for (std::uint16_t i = 0; i < 10; ++i) {
    arr[i] = {i, 0};
  }
  chip.get_display()->flip_pixel(arr);

  SDL_Delay(5000);

  SDL_Quit();
  return 0;
}
