#pragma once

#include <string>

#include <SDL.h>

#include <e/spritesheet.hpp>
#include <e/entity.hpp>

struct Collectable : Entity {
  enum Type {
    CHAINSAW,
    KEY,
    JUMPERS,
    TORCH,
    SCREEN,
    PORT
  };

  Type type;

  static std::string key(Type t);

  Collectable(int x, int y, Type t);
  void start();
  bool shouldRotate();

  Spritesheet* sprite;

  bool shouldPromptEquip = true;
  bool immovable = false;
};
