#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/input.hpp>

struct Player {
  enum Movement {
    IDLE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
  };

  Movement currentMovement = IDLE;

  Player();

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);

  Sprite* sprite;

  Input moveLeft = Input(SDL_SCANCODE_A);
  Input moveRight = Input(SDL_SCANCODE_D);
  Input moveUp = Input(SDL_SCANCODE_W);
  Input moveDown = Input(SDL_SCANCODE_S);
  Input action = Input(SDL_SCANCODE_SPACE);
};
