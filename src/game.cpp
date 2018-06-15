#include <game.hpp>

#include <stdio.h>
#include <math.h>

#include <e/csv.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

#include <e/math_util.hpp>
#include <e/collision.hpp>

bool Game::load() {
  Core::clear = SDL_Color{ 10, 10, 13, 255 };

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
  ok |= Resources::load("computer_room_wall_left.png");
  ok |= Resources::load("computer_room_wall_right.png");
  ok |= Resources::load("mountain.png");
  ok |= Resources::load("face.png");
  ok |= Resources::load("textbox.png");
  ok |= Resources::load("switchboard_indicator.png");

  ok |= Resources::loadFont("Cave-Story.ttf", 30);
  ok |= Resources::loadFont("Cave-Story.ttf", 25);
  ok |= Resources::loadFont("FifteenNarrow.ttf", 60);

  if (!ok) {
    printf("Could not load assets\n");
  }

  return ok;
}

void Game::start() {
  scene = new Scene {
    new Renderer(),
    &camera
  };

  global.scene = scene;

  // Menu will always be above entities;
  entities.localDepth = DEPTH_BELOW * DEPTH_UI;
  entities.active = false;

  global.add(&entities);
  
  // Player
  player = new Player();
  entities.add(player);

  // Menu
  Point c = player->sprite->getCenter();
  c.y -= 300;

  menu = new Menu(c.x, c.y);
  global.add(menu);

  // Game over text
  gameOver = new Entity();
  gameOver->localDepth = DEPTH_UI;
  global.add(gameOver);

  gameOverText = new Text("Game Over...", 60, camera.getWidth()/2, camera.getHeight()/2);
  gameOverText->color = { 242, 65, 65 };
  gameOverText->alignment = Text::CENTER;
  gameOverText->hud = true;

  gameOver->reg(gameOverText);
  gameOver->active = false;

  // Map
  Spritesheet* ts = new Spritesheet("tileset.png", 16, 16);
  map = new Tilemap(ts);
  map->loadLayer("map_background.csv", DEPTH_BG*2);
  map->loadLayer("map_foreground.csv", DEPTH_FG);
  darknessLayer = map->loadLayer("map_darkness.csv", DEPTH_FG + DEPTH_ABOVE);
  hiddenLayer = map->loadLayer("map_hidden.csv", DEPTH_FG + DEPTH_ABOVE);
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

    const int rowSize = 3;
    const int colSize = 5;

    bool fake[colSize][rowSize] = {
      { true, false, true },
      { false, false, true },
      { false, true, true },
      { false, false, false },
      { true, true, false },
    };

    for (int y = 0; y < colSize; y++) {
      for (int x = 0; x < rowSize; x++) {
        Floorboard* fb = new Floorboard(startX + x * size, startY + y * size, fake[y][x]);

        floorboards.add(fb);
      }
    }
  }

  // Computer room wall
  computerWall = new ComputerRoomWall(7 * ts->frameWidth, 43 * ts->frameWidth);
  computerWall->active = false;
  entities.add(computerWall);

  // Computer
  computer = new Computer();
  computer->deactivate();
  entities.add(computer);

  // Fake panel
  fakePanel = new FakePanel(14 * ts->frameWidth, 43 * ts->frameWidth);
  entities.add(fakePanel);

  // Boss
  boss = new Boss(17 * ts->frameWidth, 36 * ts->frameWidth);
  entities.add(boss);

  // load collectables
  entities.add(&collectables);
  loadCollectables("assets/lvl/map_collectables.csv");

  // Dark buffer
  darkBufferRect = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  };

  darkBuffer = SDL_CreateTexture(Core::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, darkBufferRect.w, darkBufferRect.h);
  SDL_SetTextureBlendMode(darkBuffer, SDL_BLENDMODE_MOD);
  
  // Setup camera
  camera.follow = menu->mountain;
  camera.realX = menu->mountain->x;
  camera.realY = menu->mountain->y;
}

void Game::tick(float dt) {
  SDL_Event event;

  Input::push();
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        quit = true;

        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        Input::handle(event.key);

        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        Input::handle(event.button);

        break;
      case SDL_MOUSEMOTION:
        Input::handle(event.motion);

        break;
      case SDL_TEXTINPUT:
        Input::handle(event.text);

        break;
      case SDL_TEXTEDITING:
        Input::handle(event.edit);

        break;
    }
  }

  if (player->dead && !doneTimer.running) {
    doneTimer.go();

    gameOver->active = true;
  }

  if (doneTimer.done()) {
    done = true;
  }

  if (!doneTimer.done() && doneTimer.running) {
    float pc = doneTimer.pc() * 2;

    gameOverText->alpha = pc*pc;
  }

  if (doneTimer.pc() > 0.5) {
    gameOverText->alpha = 1;
  }

  if (go.justDown()) {
    menu->exit();

    scene->camera->follow = player->sprite;

    entities.active = true;
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
  Collision::collide(player->sprite, computerWall->sprite);
  Collision::collide(player->sprite, fakePanel->sprite);

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
      case Collectable::SCREEN:
        if (!isHeld) {
          break;
        }

        if (!computer->active) {
          player->proposePrompt(player->textPower);
        }

        if (player->use.justDown()) {
          computer->activate();
        }

        break;
      case Collectable::PORT:
        if (!isHeld) {
          break;
        }

        if (!boss->awake) {
          player->proposePrompt(player->textPower);
        }

        if (player->use.justDown()) {
          boss->wakeUp();
        }

        break;
    }
  }

  Rect torchHitbox = player->torch->rect(player->eyeLine);
  torchHitbox.x += scene->camera->x;
  torchHitbox.y += scene->camera->y;
  for (auto& f : floorboards.members) {
    if (f->fake) {
      if (Collision::isOverlapping(player->sprite, f->sprite->rect())) {
        player->dead = true;
      }

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

    if (switchboard->continuous() && player->battery->hasCapacity(0.2)) {
      slidingDoor->open();
      switchboard->active = false;

      player->battery->capacity -= 0.2;
    }
  }

  if (blockade->up) {
    Rect triggerBoulder = {
      11 * 16,
      25 * 16,
      16 * 2,
      16
    };

    if (Collision::isOverlapping(player->sprite, triggerBoulder)) {
      camera.shake(5 * 1000, 0.3);
      boulder->roll();
    }

    if (Collision::isOverlapping(boulder->sprite, blockade->sprite->rect())) {
      blockade->explode();
      camera.shake(1 * 100, 1);
    }

    if (Collision::isOverlapping(boulder->sprite, player->sprite->rect())) {
      player->dead = true;
    }
  }

  if (!computerWall->active) {
    if (Collision::isOverlapping(player->sprite, computerWall->trigger)) {
      computerWall->active = true;

      computerWall->enter();
    }
  }

  if (computer->active) {
    player->battery->capacity -= 0.0001;

    if (computer->gameOver) {
      fakePanel->sprite->solid = false;

      fakePanel->active = false;

      hiddenLayerFadingIn = true;
      hiddenLayerFadeStartTime = SDL_GetTicks();

      computer->deactivate();
    }

    if (cancel.justDown()) {
      computer->deactivate();
    }
  }

  if (hiddenLayerFadingIn) {
    int now = SDL_GetTicks();
    int endTime = hiddenLayerFadeStartTime + hiddenLayerFadeDuration;
    auto& layer = map->layers[hiddenLayer];

    if (now > endTime) {
      hiddenLayerFadingIn = false;

      layer->active = false;
    }

    layer->alpha = ((float) (endTime - now)) / hiddenLayerFadeDuration;
    layer->alpha *= layer->alpha; // make fade in non-linear
  }

  if (boss->done) {
    player->dead = true;
  }

  camera.tick(dt);
  global.tick(dt);

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
      } else if (num == 5) {
        type = Collectable::Type::PORT;
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
