#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Jumper {
  Jumper(bool positive, SDL_Point socket);

  bool dirty();

  bool in();

  SDL_Point socket;

  Sprite* sprite;
};
