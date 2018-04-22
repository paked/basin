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
#include <enemy.hpp>
#include <collectable.hpp>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Player* player;
  Enemy* enemy;
  Collectable* chainsaw;

  Info* info;

  Tilemap* map;

  Camera camera;
};
