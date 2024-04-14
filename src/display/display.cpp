#include "display.hpp"
#include "config.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <memory>

display::display() noexcept
    : pixels{std::make_unique<
          std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>()},
      window{SDL_CreateWindow("SDL_RenderClear", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, DISPLAY_X * UPSCALE,
                              DISPLAY_Y * UPSCALE, 0)},
      renderer{SDL_CreateRenderer(window.get(), -1, 0)} {}

[[nodiscard]] SDL_Window *display::get_window() const noexcept {
  return this->window.get();
}

[[nodiscard]] SDL_Renderer *display::get_renderer() const noexcept {
  return this->renderer.get();
}

[[nodiscard]] std::uint8_t display::get_pixel(const pos &pos) const {
  return this->pixels->at(pos.x).at(pos.y);
}

void display::flip_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) =
      static_cast<bool>(this->pixels->at(pos.x).at(pos.y)) ? 0 : 1;
}

void display::set_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 1;
}

void display::unset_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 0;
}
