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
    TORCH
  };

  Type type;

  Collectable(int x, int y, Type t);

  void start();

  static std::string key(Type t);

  bool visible = true;
  bool shouldPromptEquip = true;

  Spritesheet* sprite;
};
