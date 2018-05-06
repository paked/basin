#include <game.hpp>

#include <stdio.h>

#include <e/csv.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

#include <e/collision.hpp>

bool Game::load() {
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
  ok |= Resources::load("maze_blockade.png");
  ok |= Resources::load("blockade_particle.png");
  ok |= Resources::load("boulder.png");
  ok |= Resources::load("explosion.png");
  ok |= Resources::loadFont("Cave-Story.ttf", 30);

  if (!ok) {
    printf("Could not load assets\n");
  }

  return ok;
}

void Game::start() {
  scene = new Scene {
    .renderer = new Renderer(),
    .camera = &camera
  };

  entities.scene = scene;

/*
  loadInfos("assets/lvl/map_infos.csv");

  blockade = new Blockade(10 * 16 * Core::scale, 23 * 16 * Core::scale);
  boulder = new Boulder(25 * 16 * Core::scale, (24 * 16 + 8) * Core::scale);

  switchboard = new Switchboard(8 * 16 * Core::scale, 11 * 16 * Core::scale);

  slidingDoor = new SlidingDoor(10 * 16 * Core::scale, 11 * 16 * Core::scale);*/

  // Player
  player = new Player();
  entities.add(player);

  // Map
  Tileset* ts = new Tileset("tileset.png", 16);
  map = new Tilemap(ts);
  map->loadLayer("map_background.csv", DEPTH_BG);
  map->loadLayer("map_foreground.csv", DEPTH_FG);
  darknessLayer = map->loadLayer("map_darkness.csv", DEPTH_FG + DEPTH_ABOVE);
  map->loadCollisionLayer("map_collision.csv");
  map->addToGroup(&entities);

  // Blockade
  blockade = new Blockade(10 * 16 * Core::scale, 23 * 16 * Core::scale);
  entities.add(blockade);

  // Boulder
  boulder = new Boulder(25 * 16 * Core::scale, (24 * 16 + 8) * Core::scale);
  entities.add(boulder);

  // load collectables
  entities.add(&collectables);
  loadCollectables("assets/lvl/map_collectables.csv");
  
  // Setup camera
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

  Collision::collide(player->sprite, map);
  Collision::collide(player->sprite, blockade->sprite->rect());

  if (!Collision::isOverlapping(player->sprite, map, darknessLayer)) {
    map->layers[darknessLayer]->active = true;
    player->torch->dark = false;
  } else {
    map->layers[darknessLayer]->active = false;
    player->torch->dark = true;
  }

  for (auto& c : collectables.members) {
    player->equipMeMaybe(c);

    bool isHeld = player->item == c;

    switch (c->type) {
      case Collectable::JUMPERS:
        /*
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

        break;*/
      case Collectable::TORCH:
        if (isHeld && !player->torch->on && player->torch->dark) {
          player->proposePrompt(player->textPower);
        }

        break;
    }
  }

  if (blockade->up) {
    SDL_Rect triggerBoulder = {.x = 11 * 16, .y = 25 * 16, .w = 16 * 2, .h = 16};
    triggerBoulder.x *= Core::scale;
    triggerBoulder.y *= Core::scale;
    triggerBoulder.w *= Core::scale;
    triggerBoulder.h *= Core::scale;

    if (Collision::isOverlapping(player->sprite, triggerBoulder)) {
      boulder->roll();
    }

    if (Collision::isOverlapping(boulder->sprite, blockade->sprite->rect())) {
      blockade->explode();
    }
  }

  entities.tick(dt);
  camera.update();
  entities.postTick();

  /*

  if (!godMode.down()) {
    Tilemap::collide(player->sprite, map);
    Sprite::collide(player->sprite, slidingDoor->rect());

    if (blockade->up) {
      Sprite::collide(player->sprite, blockade->sprite->rect());
    }
  }

  if (blockade->up) {
    SDL_Rect triggerBoulder = {.x = 11 * 16, .y = 25 * 16, .w = 16 * 2, .h = 16};
    triggerBoulder.x *= Core::scale;
    triggerBoulder.y *= Core::scale;
    triggerBoulder.w *= Core::scale;
    triggerBoulder.h *= Core::scale;

    if (Sprite::isOverlapping(player->sprite->rect(), triggerBoulder)) {
      boulder->roll();
    }

    if (Sprite::isOverlapping(boulder->sprite->rect(), blockade->sprite->rect())) {
      blockade->explode();
    }

    boulder->tick(dt);
  }

  blockade->tick(dt);

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

  player->postTick();*/
}

void Game::render(SDL_Renderer* r) {
  scene->renderer->render(r);

  /*
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

  if (blockade->up) {
    boulder->sprite->render(renderer, cam);
  }

  if (showSwitchboard) {
    switchboard->renderOverlay(renderer, cam);
  }

  map->renderForeground(renderer, camera);

  if (!player->torch->dark()) {
    darkness->renderBackground(renderer, camera);
  }

  blockade->render(renderer, camera);

  player->renderForeground(renderer, camera);*/
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

      collectables.add(new Collectable(worldX, worldY, type));
    }
  }
}


/*
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
}*/
