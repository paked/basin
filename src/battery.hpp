#pragma once

#include <SDL.h>

#include <e/spritesheet.hpp>
#include <collectable.hpp>

#include <e/component.hpp>

struct Battery : Component {
  Battery();

  void start();

  Spritesheet *sprite;
  Spritesheet *attachments;

  bool showAttachment = false;

  void attach(Collectable::Type type);
  void unattach();

  float width;
  float height;

  float capacity = 0.8f;

  bool hasCapacity(float c);

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);
};
