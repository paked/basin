#pragma once

#include <e/sprite.hpp>

struct Boulder {
  Boulder(int x, int y);

  Sprite *sprite;

  double spinSpeed = 100000;

  void roll();
  void tick(float dt);
};
