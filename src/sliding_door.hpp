#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/point.hpp>
#include <e/entity.hpp>

struct SlidingDoor : Entity {
  SlidingDoor(int x, int y);

  void start();
  void tick(float dt);

  SDL_Rect rect();

  void open();
  bool isOpen = false;

  Sprite* sprite;
};
