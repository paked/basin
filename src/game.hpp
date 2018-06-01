#pragma once

#include <vector>

#include <SDL.h>

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
#include <computer.hpp>
#include <menu.hpp>
#include <computer_room_wall.hpp>
#include <fake_panel.hpp>
#include <boss.hpp>

struct Game {
  bool load();

  void start();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  Scene* scene;
  Group<Entity> global;
  Group<Entity> entities;
  Camera camera;

  Input go = Input(SDL_SCANCODE_RETURN);
  Input cancel = Input(SDL_SCANCODE_ESCAPE, true);
  Input godMode = Input(SDL_SCANCODE_LSHIFT);

  // Transient entities
  Menu *menu;
  Player* player;
  Tilemap *map;
  Group<Collectable> collectables;
  
  // Gameplay specific entities
  Switchboard* switchboard;
  Sprite* switchboardTerminal;
  SlidingDoor* slidingDoor;
  Blockade* blockade;
  Boulder* boulder;
  Group<Floorboard> floorboards;
  ComputerRoomWall* computerWall;
  Computer *computer;
  FakePanel *fakePanel;
  Boss *boss;

  int darknessLayer;
  int hiddenLayer;

  bool hiddenLayerFadingIn = false;
  int hiddenLayerFadeStartTime = 0;
  int hiddenLayerFadeDuration = 1000;

  SDL_Texture* darkBuffer;
  SDL_Rect darkBufferRect;

  void loadCollectables(std::string fname);
};
