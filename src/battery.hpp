#pragma once

#include <SDL2/SDL.h>

#include <e/spritesheet.hpp>
#include <collectable.hpp>

#include <e/component.hpp>

struct Battery : Component {
  Battery(float x = 0, float y = 0);

  void start();

  Spritesheet *sprite;
  Spritesheet *attachments;

  bool showAttachment = false;

  void attach(Collectable::Type type);
  void unattach();

  float width;
  float height;

  float capacity = 0.3f;

  bool hasCapacity(float c);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);
};
