#include "display.hpp"
#include "config.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

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
  this->pixels->at(pos.x).at(pos.y) = this->pixels->at(pos.x).at(pos.y) ^ 1U;
  print_pixel(create_rect({.x = static_cast<uint16_t>(pos.x * UPSCALE),
                           .y = static_cast<uint16_t>(pos.y * UPSCALE)}),
              this->pixels->at(pos.x).at(pos.y));
}

void display::set_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 1;
}

void display::unset_pixel(const pos &pos) {
  this->pixels->at(pos.x).at(pos.y) = 0;
}

void display::print_pixel(const SDL_Rect &rect,
                          std::uint8_t pixel_state) const noexcept {
  SDL_SetRenderDrawColor(get_renderer(), COLOUR_MAX * (pixel_state ^ 1U),
                         COLOUR_MAX * (pixel_state ^ 1U),
                         COLOUR_MAX * (pixel_state ^ 1U), COLOUR_MAX);
  SDL_RenderFillRect(get_renderer(), &rect);
  SDL_RenderPresent(get_renderer());
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
