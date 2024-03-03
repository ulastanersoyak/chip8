#pragma once

#include <cstdint>

struct instr {
  std::uint8_t X;   // 2nd nibble
  std::uint8_t Y;   // 3rd nibble
  std::uint8_t N;   // 4th nibble
  std::uint8_t NN;  // 3rd and 4th nibble
  std::uint8_t NNN; // 2nd, 3rd and 4th nibble
};
