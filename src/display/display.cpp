#include "display.hpp"
#include "config.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

display::display() noexcept
    : pixels{std::make_unique<
          std::array<std::array<std::uint8_t, DISPLAY_Y>, DISPLAY_X>>()},
      window{SDL_CreateWindow("SDL_RenderClear", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, DISPLAY_X * UPSCALE,
                              DISPLAY_Y * UPSCALE, 0)},
      renderer{SDL_CreateRenderer(window.get(), -1, 0)} {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetRenderDrawColor(this->get_renderer(), COLOUR_MIN, COLOUR_MIN,
                         COLOUR_MIN, COLOUR_MAX);
  SDL_RenderClear(this->get_renderer());
  SDL_RenderPresent(this->get_renderer());
}

display::~display() noexcept { SDL_Quit(); }

[[nodiscard]] SDL_Window *display::get_window() const noexcept {
  return this->window.get();
}

[[nodiscard]] SDL_Renderer *display::get_renderer() const noexcept {
  return this->renderer.get();
}

[[nodiscard]] std::uint16_t display::get_pixel(const pos &pos) const {
  return this->pixels->at(pos.x).at(pos.y);
}

[[nodiscard]] bool display::flip_pixel(const pos &pos) {
  bool flipped_off = false;
  this->pixels->at(pos.x).at(pos.y) = this->pixels->at(pos.x).at(pos.y) ^ 1U;
  if (this->pixels->at(pos.x).at(pos.y) == 0U) {
    flipped_off = true;
  }
  this->draw_to_back_buffer(pos);
  return flipped_off;
}

void display::clear_window() noexcept {
  std::array<pos, DISPLAY_Y> pixel_row{};
  for (std::uint16_t row_idx = 0; auto &row : *this->pixels) {
    std::ranges::fill(row, UNSET);
    std::ranges::generate(pixel_row, [col_idx = 0, &row_idx]() mutable {
      return pos{.x = row_idx, .y = static_cast<uint16_t>(col_idx++)};
    });
    this->draw_to_back_buffer(pixel_row);
    row_idx++;
  }
}

void display::draw_to_back_buffer(const pos &pos) const {
  SDL_Rect rect = create_rect({.x = static_cast<uint16_t>(pos.x * UPSCALE),
                               .y = static_cast<uint16_t>(pos.y * UPSCALE)});
  SDL_SetRenderDrawColor(
      this->get_renderer(), COLOUR_MAX * this->pixels->at(pos.x).at(pos.y),
      COLOUR_MAX * this->pixels->at(pos.x).at(pos.y),
      COLOUR_MAX * this->pixels->at(pos.x).at(pos.y), COLOUR_MAX);
  SDL_RenderFillRect(this->get_renderer(), &rect);
  SDL_RenderPresent(this->get_renderer());
}

void display::refresh_screen() const {
  std::vector<pos> positions{};
  for (std::size_t col = 0; col < (*this->pixels).size(); col++) {
    for (std::size_t row = 0; row < (*this->pixels).at(col).size(); row++) {
      if ((*this->pixels).at(col).at(row) != 0U) {
        positions.emplace_back(col, row);
      }
    }
  }
  draw_to_back_buffer(positions);
}
