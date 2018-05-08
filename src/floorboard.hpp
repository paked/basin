#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>

struct Floorboard : Entity {
  Floorboard(int x, int y);

  void tick(float dt);

  Sprite* sprite;
};
