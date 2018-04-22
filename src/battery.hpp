#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <collectable.hpp>

struct Battery {
  Battery(int x = 0, int y = 0);

  Sprite *sprite;
  Sprite *attachments;

  bool showAttachment = false;

  void attach(Collectable::Type type);
  void unattach();

  int width;
  int height;

  float capacity = 0.3f;

  void render(SDL_Renderer *renderer, SDL_Point cam);
};
