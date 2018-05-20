#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/entity.hpp>

#include <jumper.hpp>

struct Switchboard : Entity {
  void start();

  void tick(float dt);

  bool continuous();

  Sprite* backboard;

  Jumper* inPositive;
  Jumper* inNegative;

  Jumper* outPositive;
  Jumper* outNegative;

  Jumper* pinned = nullptr;
  Point offset;
};
