#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <config.hpp>

struct Camera {
  Sprite* follow = nullptr;
  int x = 0;
  int y = 0;

  int width = SCREEN_WIDTH/GAME_SCALE;
  int height = SCREEN_HEIGHT/GAME_SCALE;

  void update();
  bool withinViewport(SDL_Rect rect);
  SDL_Point point();
};
