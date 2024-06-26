#pragma once
#ifndef INSTR_HPP
#define INSTR_HPP

#include <cstdint>

struct instr
{
  std::uint16_t full_instr;
  std::uint8_t first_nibble;
  std::uint8_t second_nibble;        // X
  std::uint8_t third_nibble;         // Y
  std::uint8_t fourth_nibble;        // N
  std::uint8_t second_byte;          // NN
  std::uint16_t except_first_nibble; // NNN
};
#endif // !INSTR_HPP
