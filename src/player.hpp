#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/input.hpp>
#include <e/camera.hpp>
#include <e/text.hpp>

#include <battery.hpp>
#include <torch.hpp>
#include <collectable.hpp>

struct Player {
  enum Movement {
    IDLE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
  };

  Movement currentMovement = IDLE;
  Torch::Direction eyeLine = Torch::DOWN;

  Player();

  void tick(float dt);
  void render(SDL_Renderer *renderer, SDL_Point cam);
  void renderForeground(SDL_Renderer *renderer, Camera cam);

  Sprite* sprite;

  Input moveLeft = Input(SDL_SCANCODE_A);
  Input moveRight = Input(SDL_SCANCODE_D);
  Input moveUp = Input(SDL_SCANCODE_W);
  Input moveDown = Input(SDL_SCANCODE_S);
  Input equip = Input(SDL_SCANCODE_E);
  Input use = Input(SDL_SCANCODE_SPACE);

  int acceleration = 40;

  Collectable* item;
  bool itemOn = false;
  bool justGotItem = false;
  bool justDroppedItem = false;
  bool hasItem = false;

  void positionItem();

  Text* equipPrompt;
  bool showEquipPrompt = false;

  bool equipMeMaybe(Collectable *c);

  Battery* battery;
  Torch* torch;
};
