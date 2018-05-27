#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>

struct FakePanel : Entity {
  FakePanel(float x, float y);
  void start();

  Sprite* sprite;
};
