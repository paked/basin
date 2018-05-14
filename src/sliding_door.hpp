#pragma once

#include <SDL2/SDL.h>

#include <e/spritesheet.hpp>
#include <e/point.hpp>
#include <e/entity.hpp>

struct SlidingDoor : Entity {
  SlidingDoor(int x, int y);

  void start();

  SDL_Rect rect();

  void open();
  bool isOpen = false;

  Spritesheet* sprite;
};
