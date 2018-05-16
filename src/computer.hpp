#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>

struct Computer : Entity {
  void start();

  std::string buffer;
  std::string input;

  Sprite* screenSprite;
};
