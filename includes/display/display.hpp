#pragma once

#include "../chip8/config.hpp"
#include "pixel_pos.hpp"

#include <array>
#include <cstdint>
#include <memory>

class display {
  std::unique_ptr<std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>
      pixels;

public:
  display() noexcept;
  [[nodiscard]] std::uint8_t get_pixel(const pos &pos);
  void flip_pixel(const pos &pos);
  void set_pixel(const pos &pos);
  void unset_pixel(const pos &pos);
};
