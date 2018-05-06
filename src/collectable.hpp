#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/entity.hpp>

struct Collectable : Entity {
  enum Type {
    CHAINSAW,
    KEY,
    JUMPERS,
    TORCH
  };

  Type type;

  Collectable(int x, int y, Type t);

  void start();
  void tick(float dt);

  static std::string key(Type t);

  bool visible = true;
  bool shouldPromptEquip = true;

  Sprite* sprite;
};
