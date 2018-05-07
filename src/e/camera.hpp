#pragma once

#include <SDL2/SDL.h>

#include <config.hpp>

struct Sprite;

struct Camera {
  void tick(float dt);

  void shake(int duration, float trauma = 0.5);

  bool withinViewport(SDL_Rect rect);
  SDL_Point point();
  SDL_Rect viewport(int buffer = 0);

  Sprite* follow = nullptr;

  int x = 0;
  int y = 0;

  int realX = 0;
  int realY = 0;

  int width = SCREEN_WIDTH;
  int height = SCREEN_HEIGHT;

  float shakeTrauma = 0;
  float shakeMax = 50;
  int shakeStopTime = 0;
  int shakeX;
  int shakeY;
};
