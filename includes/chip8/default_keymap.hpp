#pragma once
#ifndef DEFAULT_KEYMAP_HPP
#define DEFAULT_KEYMAP_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>

std::unordered_map<SDL_KeyCode, std::function<int (int)> > default_keymap{
  // { SDLK_1, foo }, { SDLK_2, foo }, { SDLK_3, foo }, { SDLK_4, foo },
  // { SDLK_q, foo }, { SDLK_w, foo }, { SDLK_e, foo }, { SDLK_r, foo },
  // { SDLK_a, foo }, { SDLK_s, foo }, { SDLK_d, foo }, { SDLK_f, foo },
  // { SDLK_z, foo }, { SDLK_x, foo }, { SDLK_c, foo }, { SDLK_v, foo }
};

#endif // !DEFAULT_KEYMAP_HPP
