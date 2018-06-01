#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/text.hpp>

struct Menu : Entity {
  Menu(float x, float y);
  void start();

  Sprite* mountain;
  Text* title;
};
