#include "chip8.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <cstdlib>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  chip8 chip{};
  [[maybe_unused]] std::array<pos, 10> arr;
  for (std::uint16_t i = 0; i < 10; ++i) {
    arr[i] = {static_cast<uint16_t>(i + 12), static_cast<uint16_t>(i + 5)};
  }

  SDL_Quit();
  return 0;
}
