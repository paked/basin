#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/point.hpp>

struct Info {
  Info(float x, float y);

  Point starting;

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);

  Sprite* sprite;

  SDL_Rect textRect;
  SDL_Texture* textTexture;

  bool showText = false;
};
