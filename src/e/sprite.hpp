#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <e/point.hpp>

struct Sprite {
  Sprite(std::string texName, float x, float y);
  Sprite(std::string texName, float x, float y, int width, int height);

  SDL_Rect rect();

  void tick(float dt);
  void render(SDL_Renderer* renderer, SDL_Point camera);

  int x;
  int y;

  int width;
  int height;

  Point nextPositionDelta;
  Point velocity;
  Point acceleration;
  Point drag = Point(0.99, 0.99);
  Point maxVelocity = Point(15, 15);

  SDL_Texture *texture;

  static void collide(Sprite* first, SDL_Rect second);
};
