#pragma once

#include <SDL2/SDL.h>

#include <config.hpp>
#include <e/rect.hpp>

struct Sprite;

struct Camera {
  void tick(float dt);

  void shake(int duration, float trauma = 0.5);

  SDL_Rect toView(Rect rect, bool global = false);
  bool withinViewport(SDL_Rect rect);
  SDL_Point point();
  SDL_Rect viewport(int buffer = 0);
  float getWidth();
  float getHeight();

  Sprite* follow = nullptr;
  float zoom = 4;

  float x = 0;
  float y = 0;

  float realX = 0;
  float realY = 0;

  int logicalWidth = SCREEN_WIDTH;
  int logicalHeight = SCREEN_HEIGHT;

  float shakeTrauma = 0;
  float shakeMax = 50;
  int shakeStopTime = 0;
  int shakeX;
  int shakeY;
};
