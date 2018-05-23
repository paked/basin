#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <e/spritesheet.hpp>
#include <e/entity.hpp>

struct Collectable : Entity {
  enum Type {
    CHAINSAW,
    KEY,
    JUMPERS,
    TORCH,
    SCREEN
  };

  Type type;

  static std::string key(Type t);

  Collectable(int x, int y, Type t);
  void start();
  bool shouldRotate();

  bool shouldPromptEquip = true;
  Spritesheet* sprite;
};
