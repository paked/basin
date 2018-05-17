#include <game.hpp>

#include <stdio.h>
#include <math.h>

#include <e/csv.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

#include <e/math_util.hpp>
#include <e/collision.hpp>

bool Game::load() {
  Core::clear = SDL_Color{.r = 10, .g = 10, .b = 13, .a = 255};

  bool ok = Resources::load("player.png");
  ok |= Resources::load("computer_gui.png");
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
  ok |= Resources::load("floorboard.png");
  ok |= Resources::load("light_small.png");
  ok |= Resources::load("safari_guy.png");
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

  // Player
  player = new Player();
  entities.add(player);

  // Map
  Spritesheet* ts = new Spritesheet("tileset.png", 16, 16);
  map = new Tilemap(ts);
  map->loadLayer("map_background.csv", DEPTH_BG*2);
  map->loadLayer("map_foreground.csv", DEPTH_FG);
  darknessLayer = map->loadLayer("map_darkness.csv", DEPTH_FG + DEPTH_ABOVE);
  map->loadCollisionLayer("map_collision.csv");
  map->addToGroup(&entities);

  // Blockade
  blockade = new Blockade(10 * 16, 23 * 16);
  entities.add(blockade);

  // Boulder
  boulder = new Boulder(25 * 16, (24 * 16 + 8));
  entities.add(boulder);

  // Switchboard
  switchboardTerminal = new Sprite("switchboard.png", 8 * 16, 11 * 16);
  switchboard = new Switchboard();
  entities.add(switchboard);
  switchboard->active = false;

  // Sliding door
  slidingDoor = new SlidingDoor(10 * 16, 11 * 16);
  entities.add(slidingDoor);

  // Floorboards
  {
    entities.add(&floorboards);

    int size = 32;
    int tileSize = ts->frameWidth;

    int startX = 1 * tileSize;
    int startY = 31 * tileSize;

    int rowSize = 3;
    int colSize = 5;

    bool fake[colSize][rowSize] = {
      { false, true, false },
      { true, true, false },
      { true, false, false },
      { true, true, true },
      { false, false, true },
    };

    for (int y = 0; y < colSize; y++) {
      for (int x = 0; x < rowSize; x++) {
        Floorboard* fb = new Floorboard(startX + x * size, startY + y * size, fake[y][x]);

        floorboards.add(fb);
      }
    }
  }

  computer = new Computer();
  entities.add(computer);

  // load collectables
  entities.add(&collectables);
  loadCollectables("assets/lvl/map_collectables.csv");

  // Dark buffer
  darkBufferRect = {
    .x = 0,
    .y = 0,
    .w = SCREEN_WIDTH,
    .h = SCREEN_HEIGHT
  };

  darkBuffer = SDL_CreateTexture(Core::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, darkBufferRect.w, darkBufferRect.h);
  SDL_SetTextureBlendMode(darkBuffer, SDL_BLENDMODE_MOD);
  
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

  if (godMode.justDown()) {
    player->sprite->solid = false;
  }

  if (godMode.justUp()) {
    player->sprite->solid = true;
  }

  Collision::collide(player->sprite, map);
  Collision::collide(player->sprite, blockade->sprite);
  Collision::collide(player->sprite, slidingDoor->sprite);

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
        if (!Collision::isOverlapping(c->sprite, switchboardTerminal->rect())) {
          // we only care about jumpers if they're touching the terminal

          break;
        }

        if (isHeld) {
          player->proposePrompt(player->textInsert);
        }

        if (player->justDroppedItem && player->lastDroppedItem == c) {
          switchboard->active = true;
        }

        break;
      case Collectable::TORCH:
        if (isHeld && !player->torch->on && player->torch->dark) {
          player->proposePrompt(player->textPower);
        }

        break;
    }
  }

  Rect torchHitbox = player->torch->rect(player->eyeLine);
  torchHitbox.x += scene->camera->x;
  torchHitbox.y += scene->camera->y;
  for (auto& f : floorboards.members) {
    if (!f->fake) {
      continue;
    }

    int hotspotSize = 10;
    Rect floorboardHitbox = f->sprite->rect();
    floorboardHitbox.x += hotspotSize/2;
    floorboardHitbox.y += hotspotSize/2;
    floorboardHitbox.w = hotspotSize;
    floorboardHitbox.h = hotspotSize;

    if (!Collision::isOverlapping(floorboardHitbox, torchHitbox)) {
      continue;
    }

    f->trigger();
  }

  if (switchboard->active) {
    if (cancel.justDown()) {
      switchboard->active = false;
    }

    if (switchboard->continuous()) {
      slidingDoor->open();
      switchboard->active = false;
    }
  }

  if (blockade->up) {
    Rect triggerBoulder = {
      .x = 11 * 16,
      .y = 25 * 16,
      .w = 16 * 2,
      .h = 16
    };

    if (Collision::isOverlapping(player->sprite, triggerBoulder)) {
      camera.shake(5 * 1000, 0.3);
      boulder->roll();
    }

    if (Collision::isOverlapping(boulder->sprite, blockade->sprite->rect())) {
      blockade->explode();
      camera.shake(1 * 100, 1);
    }
  }

  camera.tick(dt);
  entities.tick(dt);

  // TODO fix this
  // switchboardTerminal->job(scene);

  entities.postTick();
}

void Game::render(SDL_Renderer* r) {
   if (player->torch->dark) {
    // Setup render texture
    SDL_SetRenderTarget(r, darkBuffer);

    SDL_SetRenderDrawColor(r, 0, 0, 0, (player->torch->dark ? 255 : 0));
    SDL_RenderClear(r);

    for (auto& fb : floorboards.members) {
      fb->glow();
    }

    if (player->torch->on) {
      player->torch->beamIn(player->eyeLine);
    }

    // Revert render texture back to regular screen
    SDL_SetRenderTarget(r, NULL);

    RenderJob j;
    j.depth = DEPTH_UI + DEPTH_BELOW;
    j.tex = darkBuffer;
    j.src = darkBufferRect;
    j.dst = darkBufferRect;

    scene->renderer->queue.push(j);
  }

  // Actually render the scene
  scene->renderer->render(r);
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
      } else if (num == 4) {
        type = Collectable::Type::SCREEN;
      } else {
        printf("Error loading collectables CSV: invalid collectable type\n");

        continue;
      }

      int worldX = (x * 16 + 12);
      int worldY = (y * 16 + 12);

      collectables.add(new Collectable(worldX, worldY, type));
    }
  }
}
