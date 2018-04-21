#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/camera.hpp>
#include <e/input.hpp>
#include <e/tilemap.hpp>

#include <config.hpp>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Sprite* player;

  Input moveLeft = Input(SDL_SCANCODE_A);
  Input moveRight = Input(SDL_SCANCODE_D);
  Input moveUp = Input(SDL_SCANCODE_W);
  Input moveDown = Input(SDL_SCANCODE_S);

  Sprite* wall;

  Tilemap* map;

  Camera camera;
};
