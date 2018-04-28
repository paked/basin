#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <config.hpp>

struct Camera {
  Sprite* follow = nullptr;

  int x = 0;
  int y = 0;

  int width = SCREEN_WIDTH;
  int height = SCREEN_HEIGHT;

  void update();
  bool withinViewport(SDL_Rect rect);
  SDL_Point point();

  SDL_Rect viewport(int buffer = 0);
};
