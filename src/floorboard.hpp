#pragma once

#include <e/entity.hpp>
#include <e/spritesheet.hpp>

struct Floorboard : Entity {
  Floorboard(int x, int y, bool fake);

  void tick(float dt);

  void trigger();
  void glow();

  Sprite* sprite;
  Sprite* light;

  bool fake;
  bool glowing = false;

  int glowDuration = 500;
  int glowingStartTime = 0;
};
