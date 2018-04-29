#pragma once

#include <vector>

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
#include <sliding_door.hpp>
#include <switchboard.hpp>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Player* player;
  Enemy* enemy;

  std::vector<Collectable*> collectables;
  void tickCollectables(float dt);
  std::vector<Info*> infos;

  SlidingDoor* slidingDoor;

  Switchboard* switchboard;
  bool showSwitchboard = false;

  Tilemap* map;

  Camera camera;

  void loadCollectables(std::string fname);
  void loadInfos(std::string fname);

  Input cancel = Input(SDL_SCANCODE_ESCAPE);
  Input godMode = Input(SDL_SCANCODE_TAB);
};
