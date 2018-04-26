#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Collectable {
  enum Type {
    CHAINSAW,
    KEY,
    JUMPERS
  };

  Type type;

  Collectable(int x, int y, Type t);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);

  static std::string key(Type t);

  bool active = true;

  Sprite* sprite;
};
