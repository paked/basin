#include <game.hpp>

#include <stdio.h>

#include <e/csv.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

void Game::load() {
  Core::clear = SDL_Color{.r = 10, .g = 10, .b = 13, .a = 255};

  bool ok = Resources::load("player.png");
  ok |= Resources::load("wall.png");
  ok |= Resources::load("tileset.png");
  ok |= Resources::load("info.png");
  ok |= Resources::load("battery.png");
  ok |= Resources::load("walk_test.png");
  ok |= Resources::load("slime.png");
  ok |= Resources::load("collectables.png");
  ok |= Resources::load("sliding_door_vert.png");
  ok |= Resources::load("switchboard.png");
  ok |= Resources::load("battery_attachments.png");
  ok |= Resources::load("switchboard_gui.png");
  ok |= Resources::load("switchboard_gui_jumpers.png");
  ok |= Resources::load("switchboard_gui_overlay.png");
  ok |= Resources::load("torch_beam.png");
  ok |= Resources::loadFont("Cave-Story.ttf", 30);

  if (!ok) {
    printf("Could not load assets\n");
  }

  map = new Tilemap();
  map->loadTileset("tileset.png");
  map->loadBackground("assets/lvl/map_background.csv");
  map->loadForeground("assets/lvl/map_foreground.csv");
  map->loadCollision("assets/lvl/map_collision.csv");

  loadCollectables("assets/lvl/map_collectables.csv");
  loadInfos("assets/lvl/map_infos.csv");

  switchboard = new Switchboard(8 * 16 * Core::scale, 11 * 16 * Core::scale);

  player = new Player();

  slidingDoor = new SlidingDoor(10 * 16 * Core::scale, 11 * 16 * Core::scale);

  camera.follow = player->sprite;
}

void Game::tick(float dt) {
  SDL_Event event;

  Input::push();
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = true;
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      Input::handle(event.key);
    } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
      Input::handle(event.button);
    } else if (event.type == SDL_MOUSEMOTION) {
      Input::handle(event.motion);
    }
  }

  SDL_Rect darkZone = {.x = 1 * 16, .y = 16 * 16, .w = 12 * 16, .h = 9 * 16 };
  darkZone.x *= Core::scale;
  darkZone.y *= Core::scale;
  darkZone.w *= Core::scale;
  darkZone.h *= Core::scale;

  if (Sprite::isOverlapping(player->sprite->rect(), darkZone)) {
    player->torch->darkness = 1.0;
  } else {
    player->torch->darkness = 0.0;
  }

  if (!godMode.down()) {
    Tilemap::collide(player->sprite, map);
    Sprite::collide(player->sprite, slidingDoor->rect());
  }



  tickCollectables(dt);

  for (auto info : infos) {
    info->showText = Sprite::isOverlapping(player->sprite->rect(), info->sprite->rect());

    info->tick(dt);
  }

  player->tick(dt);
  slidingDoor->tick(dt);

  // TODO: flatten this
  if (showSwitchboard) {
    if (switchboard->continuous()) {
      player->proposePrompt(player->textPower);

      if (player->use.justDown() && player->battery->hasCapacity(0.1)) {
        player->battery->capacity -= 0.1;

        slidingDoor->open();

        showSwitchboard = false;
        player->busy = false;
      }
    }

    if (cancel.justDown()) {
      showSwitchboard = false;
      player->busy = false;
    }

    switchboard->tick(dt);
  }

  camera.update();

  player->postTick();
}

void Game::render(SDL_Renderer* renderer) {
  SDL_Point cam = camera.point();

  map->renderBackground(renderer, camera);

  player->render(renderer, cam);

  for (auto collectable : collectables) {
    collectable->render(renderer, cam);
  }

  for (auto info : infos) {
    info->render(renderer, cam);
  }

  switchboard->render(renderer, cam);

  slidingDoor->render(renderer, cam);

  map->renderForeground(renderer, camera);
  player->renderForeground(renderer, camera);

  if (showSwitchboard) {
    switchboard->renderOverlay(renderer, cam);
  }
}

void Game::tickCollectables(float dt) {
  for (auto c : collectables) {
    c->tick(dt);

    player->equipMeMaybe(c);

    bool isHeld = player->item == c;

    switch (c->type) {
      case Collectable::KEY:
        if (Sprite::isOverlapping(c->sprite->rect(), slidingDoor->rect())) {
          slidingDoor->open();
        }

        break;
      case Collectable::JUMPERS:
        if (!Sprite::isOverlapping(c->sprite->rect(), switchboard->terminal->rect())) {
          // add implicit is-overlapping-with-player condition to rest of scope.
          break;
        }

        if (isHeld) {
          player->proposePrompt(player->textInsert);
        }

        if (player->justDroppedItem && player->lastDroppedItem == c) {
          showSwitchboard = true;
          player->busy = true;
        }

        break;
    }
  }
}

void Game::loadCollectables(std::string fname) {
  CSV csv(fname);

  auto data = csv.getDataInt();

  for (int y = 0; y < data.size(); y++) {
    auto row = data[y];

    for (int x = 0; x < row.size(); x++) {
      int num = row[x];
      Collectable::Type type;

      if (num < 0) {
        continue;
      } else if (num == 0) {
        type = Collectable::Type::CHAINSAW;
      } else if (num == 1) {
        type = Collectable::Type::KEY;
      } else if (num == 2) {
        type = Collectable::Type::JUMPERS;
      } else if (num == 3) {
        type = Collectable::Type::TORCH;
      } else {
        printf("Error loading collectables CSV: invalid collectable type\n");

        continue;
      }

      int worldX = (x * 16 + 12) * Core::scale;
      int worldY = (y * 16 + 12) * Core::scale;

      collectables.push_back(new Collectable(worldX, worldY, type));
    }
  }
}

void Game::loadInfos(std::string fname) {
  CSV csv(fname);

  auto data = csv.getData();

  for (auto row : data) {
    if (row.size() != 3) {
      for (auto c : row) {
        printf("%s |", c.c_str());
      }

      printf("\nError loading infos: malformed row (size is %d, when should be 3)\n", (int)row.size());

      continue;
    }

    int x = stoi(row[0]) * 16 + 12;
    int y = stoi(row[1]) * 16 + 12;
    std::string msg = row[2];

    infos.push_back(new Info(x * Core::scale, y * Core::scale, msg));
  }
}
