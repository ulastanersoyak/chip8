#pragma once

#include "../chip8/config.hpp"
#include "pixel_pos.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <cstdint>
#include <memory>

class display final {
  struct window_destructor final {
    void operator()(SDL_Window *w) noexcept { SDL_DestroyWindow(w); }
  };

  struct renderer_destructor final {
    void operator()(SDL_Renderer *r) noexcept { SDL_DestroyRenderer(r); }
  };

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
  void clear_window() noexcept;
};
