#pragma once

#include "../chip8/config.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <memory>

template <typename container>
concept pos_container = std::ranges::range<container>
                        && std::is_same_v<typename container::value_type, pos>;

class display final
{
  struct window_destructor final
  {
    void
    operator() (SDL_Window *window_) noexcept
    {
      SDL_DestroyWindow (window_);
    }
  };

  struct renderer_destructor final
  {
    void
    operator() (SDL_Renderer *renderer_) noexcept
    {
      SDL_DestroyRenderer (renderer_);
    }
  };

  void draw_to_back_buffer (const pos &pos) const;

  void
  draw_to_back_buffer (const pos_container auto &positions) const
  {
    SDL_Rect rect;
    for (const auto &pos : positions)
      {
        rect = create_rect ({ .x = static_cast<uint16_t> (pos.x * UPSCALE),
                              .y = static_cast<uint16_t> (pos.y * UPSCALE) });
        SDL_SetRenderDrawColor (
            this->get_renderer (),
            COLOUR_MAX * this->pixels->at (pos.x).at (pos.y),
            COLOUR_MAX * this->pixels->at (pos.x).at (pos.y),
            COLOUR_MAX * this->pixels->at (pos.x).at (pos.y), COLOUR_MAX);
        SDL_RenderFillRect (this->get_renderer (), &rect);
      }
    SDL_RenderPresent (this->get_renderer ());
  }

  [[nodiscard]] static inline SDL_Rect
  create_rect (const pos &pos) noexcept
  {
    return SDL_Rect{
      .x = pos.x, .y = pos.y, .w = PIXEL_WIDTH, .h = PIXEL_HEIGHT
    };
  }

  std::unique_ptr<std::array<std::array<std::uint8_t, DISPLAY_Y>, DISPLAY_X> >
      pixels;
  std::unique_ptr<SDL_Window, window_destructor> window;
  std::unique_ptr<SDL_Renderer, renderer_destructor> renderer;

public:
  display () noexcept;
  ~display () noexcept;
  display (const display &) = delete;
  display (display &&) = default;
  display &operator= (const display &) = delete;
  display &operator= (display &&) = default;

  [[nodiscard]] SDL_Window *get_window () const noexcept;
  [[nodiscard]] SDL_Renderer *get_renderer () const noexcept;

  void clear_window () noexcept;

  [[nodiscard]] bool
  flip_pixel (const pos_container auto &positions)
  {
    bool flipped_off = false;
    for (const auto &pos : positions)
      {
        this->pixels->at (pos.x).at (pos.y)
            = this->pixels->at (pos.x).at (pos.y) ^ 1U;
        if (this->pixels->at (pos.x).at (pos.y) == 0U)
          {
            flipped_off = true;
          }
      }
    this->draw_to_back_buffer (positions);
    return flipped_off;
  }
  void refresh_screen () const;
};
