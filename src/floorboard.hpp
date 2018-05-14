#pragma once

#include <e/entity.hpp>
#include <e/spritesheet.hpp>

struct Floorboard : Entity {
  Floorboard(int x, int y);

  void tick(float dt);

  void glow();

  Sprite* sprite;
  Sprite* light;

  bool fake = true;
  bool glowing = true;
};
