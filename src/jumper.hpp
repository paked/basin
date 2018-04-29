#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Jumper {
  Jumper(bool positive, SDL_Point socket);
  void render(SDL_Renderer* renderer, SDL_Point cam);

  void generateColliders();

  bool dirty();
  bool in();

  SDL_Rect rect();

  int socketPadding = 2;
  SDL_Rect socketLeftPad;
  SDL_Rect socketRightPad;
  SDL_Rect socketTopPad;
  SDL_Rect socketInPad;

  SDL_Point socket;

  Sprite* sprite;
};
