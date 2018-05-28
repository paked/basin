#pragma once

#include <SDL.h>

#include <e/spritesheet.hpp>
#include <e/point.hpp>
#include <e/entity.hpp>
#include <e/rect.hpp>

struct SlidingDoor : Entity {
  SlidingDoor(float x, float y);

  void start();

  Rect rect();

  void open();
  bool isOpen = false;

  Spritesheet* sprite;
};
