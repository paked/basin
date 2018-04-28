#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Torch {
  Torch();

  // TODO: make this an engine constant
  enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  Direction direction;

  void render(SDL_Renderer* renderer);

  Sprite* beam;
  SDL_Texture* buffer;

  float darkness = 0.0;

  void pre();
  void post();

  void beamIn(Direction d);
};
