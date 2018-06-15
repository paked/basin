#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/text.hpp>
#include <e/timer.hpp>

struct Menu : Entity {
  Menu(float x, float y);

  void start();
  void tick(float dt);

  void exit();

  Sprite* mountain;
  Text* title;
  Text* pressEnter;

  Timer exitTimer;
  Timer enterTimer = Timer(400);
};
