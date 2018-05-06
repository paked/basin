#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/camera.hpp>
#include <e/input.hpp>
#include <e/tilemap.hpp>
#include <e/renderer.hpp>
#include <e/scene.hpp>
#include <e/tilelayer.hpp>

#include <e/group.hpp>

#include <config.hpp>

#include <player.hpp>
#include <info.hpp>
#include <enemy.hpp>
#include <collectable.hpp>
#include <sliding_door.hpp>
#include <switchboard.hpp>
#include <boulder.hpp>
#include <blockade.hpp>

struct Game {
  bool load();

  void start();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Scene* scene;
  Group* entities;

  Player* player;
  Tilemap *map;
  Camera camera;

  /*
  std::vector<Collectable*> collectables;
  void tickCollectables(float dt);
  void loadCollectables(std::string fname);
  void loadInfos(std::string fname);

  Enemy* enemy;

  Blockade* blockade;

  Boulder* boulder;

  std::vector<Collectable*> collectables;
  void tickCollectables(float dt);
  std::vector<Info*> infos;

  SlidingDoor* slidingDoor;

  Switchboard* switchboard;
  bool showSwitchboard = false;

  Tilemap* darkness;
  Tilemap* map;

  Camera camera;

  void loadCollectables(std::string fname);
  void loadInfos(std::string fname);

  Input cancel = Input(SDL_SCANCODE_ESCAPE);
  Input godMode = Input(SDL_SCANCODE_TAB);
  */
};
