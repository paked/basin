#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/point.hpp>
#include <e/text.hpp>

struct SlidingDoor {
  SlidingDoor(int x, int y);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);

  SDL_Rect rect();

  void open();
  bool isOpen = false;

  Sprite* sprite;

  Sprite* switchboard;
};
