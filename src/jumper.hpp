#pragma once

#include <SDL.h>

#include <e/spritesheet.hpp>
#include <e/rect.hpp>
#include <e/collision.hpp>

struct Jumper {
  Jumper(bool positive, Point socket);
  void render(SDL_Renderer* renderer, Camera* cam);

  void tick(float dt);

  void generateColliders();

  bool dirty();
  bool in();

  Rect rect();

  int socketPadding = 2;
  Rect socketLeftPad;
  Rect socketRightPad;
  Rect socketTopPad;
  Rect socketInPad;

  Point socket;

  Spritesheet* sprite;
  Spritesheet* indicator;
};
