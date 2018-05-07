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
  Group<Entity> entities;
  Camera camera;

  Input cancel = Input(SDL_SCANCODE_ESCAPE);

  Player* player;
  Tilemap *map;
  Blockade* blockade;
  Boulder* boulder;
  Group<Collectable> collectables;

  Switchboard* switchboard;
  Sprite* switchboardTerminal;

  SlidingDoor* slidingDoor;

  int darknessLayer;

  void loadCollectables(std::string fname);
};
