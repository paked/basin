#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/entity.hpp>

#include <jumper.hpp>

struct Switchboard : Entity {
  void start();

  void tick(float dt);
  void renderOverlay(SDL_Renderer* renderer, SDL_Point cam);

  bool continuous();

  Sprite* backboard;
  Sprite* overlay;

  Jumper* inPositive;
  Jumper* inNegative;

  Jumper* outPositive;
  Jumper* outNegative;

  Jumper* pinned = nullptr;
  SDL_Point offset;
};
