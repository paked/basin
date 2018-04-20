#pragma once

#include <SDL2/SDL.h>

#include <e/sprite.hpp>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Sprite* player;

  bool playerMoveLeft = false;
  bool playerMoveRight = false;
  bool playerMoveUp = false;
  bool playerMoveDown = false;
};
