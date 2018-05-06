#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <collectable.hpp>

#include <e/component.hpp>

struct Battery : Component{
  Battery(int x = 0, int y = 0);

  void start();

  Sprite *sprite;
  Sprite *attachments;

  bool showAttachment = false;

  void attach(Collectable::Type type);
  void unattach();

  int width;
  int height;

  float capacity = 0.3f;

  bool hasCapacity(float c);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);
};
