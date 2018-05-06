#pragma once

#include <e/sprite.hpp>
#include <e/entity.hpp>

struct Boulder : Entity {
  Boulder(int x, int y);

  Sprite *sprite;

  double spinSpeed = 100000;

  void roll();
  void tick(float dt);
};
