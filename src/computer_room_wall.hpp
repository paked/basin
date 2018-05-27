#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/rect.hpp>

struct ComputerRoomWall : Entity {
  ComputerRoomWall(float x, float y);

  void start();
  void tick(float dt);
  void enter();

  Rect trigger;

  float startX;
  float distX = 3 * 16;

  int enterDuration = 500;
  int bounceStartTime = 0;

  bool bouncing = false;

  Sprite* sprite;
};
