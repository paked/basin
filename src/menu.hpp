#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/text.hpp>

struct Menu : Entity {
  void start();
  void tick(float dt);

  void exit();

  Sprite* mountain;
  Text* title;

  bool done = false;
  bool exiting = false;
  int exitingStartTime = 0;
  int exitingDuration = 1000;
  int yOffsetMountainStart = 0;
  int yOffsetTotal = -300;
};
