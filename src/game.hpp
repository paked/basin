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
#include <floorboard.hpp>

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
  Input godMode = Input(SDL_SCANCODE_LSHIFT);

  Player* player;
  Tilemap *map;
  Blockade* blockade;
  Boulder* boulder;
  Group<Collectable> collectables;
  Group<Floorboard> floorboards;

  Switchboard* switchboard;
  Sprite* switchboardTerminal;

  SlidingDoor* slidingDoor;

  Sprite* light;

  int darknessLayer;

  SDL_Texture* darkBuffer;
  SDL_Rect darkBufferRect;

  void loadCollectables(std::string fname);
};
