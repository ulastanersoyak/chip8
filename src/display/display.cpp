#include "display.hpp"
#include "../chip8/config.hpp"

#include <array>
#include <cstdint>
#include <memory>

display::display() noexcept
    : pixels{std::make_unique<
          std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>()} {}

[[nodiscard]] std::uint8_t display::get_pixel(const pos &pos) {
  return this->pixels->at(pos.x).at(pos.y);
}

void display::flip_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = !this->pixels->at(pos.x).at(pos.y);
}

void display::set_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 1;
}

void display::unset_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 0;
}
