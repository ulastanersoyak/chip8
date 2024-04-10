#pragma once

#include <cstdint>
#define BLACK 0, 0, 0, 0
#define WHITE 1, 1, 1, 1

struct pos final {
  std::uint8_t x{0};
  std::uint8_t y{0};
};
