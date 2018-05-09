#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Torch {
  Torch();

  // TODO: make this an engine constant
  enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  Direction direction;

  void beamIn(Direction d);

  Sprite* beam;

  bool on = false;

  bool dark = false;
};
