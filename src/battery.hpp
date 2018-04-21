#pragma once

#include <SDL2/SDL.h>

struct Battery {
  Battery(int x = 0, int y = 0);

  int x;
  int y;

  int width;
  int height;

  float capacity = 0.0f;

  SDL_Texture *texture;
  int frames = 3;

  void render(SDL_Renderer *renderer);
};
