#pragma once

#include <SDL2/SDL.h>

#include <e/spritesheet.hpp>
#include <e/input.hpp>
#include <e/camera.hpp>
#include <e/text.hpp>
#include <e/entity.hpp>

#include <battery.hpp>
#include <torch.hpp>
#include <collectable.hpp>

struct Player : Entity {
  enum Movement {
    IDLE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
  };

  void tick(float dt);
  void start();
  void postTick();

  Movement currentMovement = IDLE;
  Torch::Direction eyeLine = Torch::DOWN;

  Spritesheet* sprite;

  Input moveLeft = Input(SDL_SCANCODE_A);
  Input moveRight = Input(SDL_SCANCODE_D);
  Input moveUp = Input(SDL_SCANCODE_W);
  Input moveDown = Input(SDL_SCANCODE_S);
  Input equip = Input(SDL_SCANCODE_E);
  Input use = Input(SDL_SCANCODE_SPACE);

  bool busy = false;

  int acceleration = 40;

  Collectable* item;
  bool itemOn = false;
  bool justGotItem = false;
  bool justDroppedItem = false;
  Collectable* lastDroppedItem = nullptr;
  bool hasItem = false;

  void positionItem();

  Text* textEquip;
  Text* textInsert;
  Text* textPower;

  bool equipMeMaybe(Collectable *c);

  bool promptClear = false;
  void proposePrompt(Text *text);

  bool doPrompt = false;
  SDL_Point promptPosition = { 400, 100};
  Text *promptText;
  bool promptOn = true;
  int promptNextToggle;
  int promptToggleFrequency = 500;

  Battery* battery;
  Torch* torch;
};
