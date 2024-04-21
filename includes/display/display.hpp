#pragma once

#include "../chip8/config.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <memory>

template <typename container>
concept pos_container = std::ranges::range<container> &&
                        std::is_same_v<typename container::value_type, pos>;

class display final {
  struct window_destructor final {
    void operator()(SDL_Window *window_) noexcept {
      SDL_DestroyWindow(window_);
    }
  };

  struct renderer_destructor final {
    void operator()(SDL_Renderer *renderer_) noexcept {
      SDL_DestroyRenderer(renderer_);
    }
  };

  void draw_to_back_buffer(const pos &pos) const;

  void draw_to_back_buffer(const pos_container auto &positions) const {
    SDL_Rect rect;
    for (const auto &pos : positions) {
      rect = create_rect({.x = static_cast<uint16_t>(pos.x * UPSCALE),
                          .y = static_cast<uint16_t>(pos.y * UPSCALE)});
      SDL_SetRenderDrawColor(
          this->get_renderer(), COLOUR_MAX * this->pixels->at(pos.x).at(pos.y),
          COLOUR_MAX * this->pixels->at(pos.x).at(pos.y),
          COLOUR_MAX * this->pixels->at(pos.x).at(pos.y), COLOUR_MAX);
      SDL_RenderFillRect(this->get_renderer(), &rect);
    }
    SDL_RenderPresent(this->get_renderer());
  }

  [[nodiscard]] static SDL_Rect create_rect(const pos &pos) noexcept {
    return SDL_Rect{
        .x = pos.x, .y = pos.y, .w = PIXEL_WIDTH, .h = PIXEL_HEIGHT};
  }

  std::unique_ptr<std::array<std::array<std::uint8_t, DISPLAY_X>, DISPLAY_Y>>
      pixels;
  std::unique_ptr<SDL_Window, window_destructor> window;
  std::unique_ptr<SDL_Renderer, renderer_destructor> renderer;

public:
  display() noexcept;
  [[nodiscard]] SDL_Window *get_window() const noexcept;
  [[nodiscard]] SDL_Renderer *get_renderer() const noexcept;
  [[nodiscard]] std::uint8_t get_pixel(const pos &pos) const;
  void flip_pixel(const pos &pos);
  void set_pixel(const pos &pos);
  void unset_pixel(const pos &pos);

  void flip_pixel(const pos_container auto &positions) {
    for (const auto &pos : positions) {
      this->pixels->at(pos.x).at(pos.y) =
          this->pixels->at(pos.x).at(pos.y) ^ 1U;
    }
    this->draw_to_back_buffer(positions);
  }

  void set_pixel(const pos_container auto &positions) {
    for (const auto &pos : positions) {
      this->pixels->at(pos.x).at(pos.y) = 1;
    }
    this->draw_to_back_buffer(positions);
  }

  void unset_pixel(const pos_container auto &positions) {
    for (const auto &pos : positions) {
      this->pixels->at(pos.x).at(pos.y) = 0;
    }
    this->draw_to_back_buffer(positions);
  }
  void clear_window() noexcept;
};
