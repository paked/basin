#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/camera.hpp>
#include <e/input.hpp>
#include <e/tilemap.hpp>

#include <config.hpp>

#include <player.hpp>
#include <info.hpp>
#include <battery.hpp>
#include <enemy.hpp>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Player* player;
  Enemy* enemy;

  Input action = Input(SDL_SCANCODE_SPACE);

  Info* info;
  Battery* battery;

  Tilemap* map;

  Camera camera;
};
