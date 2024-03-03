#include "chip8.hpp"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  chip8 x{};
  auto i = x.fetch();
  std::cout << i.X << '\n'
            << i.Y << '\n'
            << i.N << '\n'
            << i.NN << '\n'
            << i.NNN;
  return 0;
}
