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
  ok |= Resources::loadFont("Cave-Story.ttf", 15);

  if (!ok) {
    printf("Could not load assets\n");
  }

  map = new Tilemap();
  map->loadTileset("tileset.png");
  map->loadBackground("assets/lvl/level_background.csv");
  map->loadForeground("assets/lvl/level_foreground.csv");
  map->loadCollision("assets/lvl/level_collision.csv");

  loadCollectables("assets/lvl/level_collectables.csv");
  loadInfos("assets/lvl/level_infos.csv");

  switchboard = new Switchboard(camera.width/2, camera.height/2);
  switchboardTerminal = new Sprite("switchboard.png", 28 * 16, 8 * 16);

  player = new Player();

  enemy = new Enemy(33 * 16, 10 * 16);
  slidingDoor = new SlidingDoor(27 * 16, 9 * 16);

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

  if (Sprite::isOverlapping(player->sprite->rect(), darkZone)) {
    player->torch->darkness = 1.0;
  } else {
    player->torch->darkness = 0.0;
  }

  Tilemap::collide(player->sprite, map);
  Sprite::collide(player->sprite, slidingDoor->rect());

  for (auto c : collectables) {
    c->tick(dt);

    player->equipMeMaybe(c);

    if (c->type == Collectable::KEY) {
      if (Sprite::isOverlapping(c->sprite->rect(), slidingDoor->rect())) {
          slidingDoor->open();
      }
    } else if (c->type == Collectable::JUMPERS) {
      showSwitchboard = Sprite::isOverlapping(c->sprite->rect(), switchboardTerminal->rect());
    }
  }

  for (auto info : infos) {
    info->showText = Sprite::isOverlapping(player->sprite->rect(), info->sprite->rect());

    info->tick(dt);
  }

  player->tick(dt);
  slidingDoor->tick(dt);
  enemy->tick(dt);

  if (showSwitchboard) {
    switchboard->tick(dt);

    if (switchboard->continuous()) {
      slidingDoor->open();
    }
  }

  camera.update();
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

  switchboardTerminal->render(renderer, cam);

  enemy->render(renderer, cam);

  slidingDoor->render(renderer, cam);

  map->renderForeground(renderer, camera);
  player->renderForeground(renderer, camera);

  if (showSwitchboard) {
    switchboard->render(renderer, cam);
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

      collectables.push_back(new Collectable(x * 16, y * 16, type));
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

    int x = stoi(row[0]);
    int y = stoi(row[1]);
    std::string msg = row[2];

    infos.push_back(new Info(x * 16, y * 16, msg));
  }
}
