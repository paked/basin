#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/component.hpp>

struct Torch : Component {
  // TODO: make this an engine constant
  enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  Torch();

  void beamIn(Direction d);
  Rect rect(Direction d);

  Sprite* beam;

  bool on = false;

  bool dark = false;
};
