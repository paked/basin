#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Collectable {
  Collectable(int x, int y);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);

  bool active = true;

  Sprite* sprite;
};
