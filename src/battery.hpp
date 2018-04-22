#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Battery {
  Battery(int x = 0, int y = 0);

  Sprite *sprite;

  int width;
  int height;

  float capacity = 0.0f;

  void render(SDL_Renderer *renderer);
};
