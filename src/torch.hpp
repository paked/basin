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

  void pre();
  void beamIn(Direction d);
  void post();

  void job(Scene *scene);

  Sprite* beam;
  SDL_Texture* buffer;
  SDL_Rect bufferSRC;

  bool on = false;

  bool dark = false;
};
