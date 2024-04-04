#include "chip8.hpp"
#include <SDL2/SDL.h>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  chip8 x{};
  const auto i{x.fetch()};
  std::cout << i.X << '\n'
            << i.Y << '\n'
            << i.N << '\n'
            << i.NN << '\n'
            << i.NNN;
  [[maybe_unused]] const auto window = SDL_CreateWindow(
      "test title", 100, 100, 10, 10, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  return 0;
}
