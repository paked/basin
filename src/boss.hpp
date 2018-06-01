#pragma once

#include <e/entity.hpp>
#include <e/spritesheet.hpp>

struct Boss : Entity {
  Boss(float x, float y);

  void start();

  void tick(float dt);

  void wakeUp();

  Spritesheet* sprite;

  bool awake = false;
};
