#pragma once

#include <SDL.h>

#include <config.hpp>
#include <e/rect.hpp>
#include <e/point.hpp>

struct Sprite;

struct Camera {
  void tick(float dt);

  void shake(int duration, float trauma = 0.5);

  SDL_Rect toView(Rect rect, bool global = false);
  Rect viewport(float buffer = 0);
  bool withinViewport(Rect rect);
  Point point();
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
  float shakeMax = 25;
  int shakeStopTime = 0;
  int shakeX;
  int shakeY;
};
