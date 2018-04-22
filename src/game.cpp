#include <game.hpp>

#include <stdio.h>

#include <e/csv.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

void Game::load() {
  bool ok = Resources::load("player.png");
  ok |= Resources::load("wall.png");
  ok |= Resources::load("tileset.png");
  ok |= Resources::load("info.png");
  ok |= Resources::load("battery.png");
  ok |= Resources::load("walk_test.png");
  ok |= Resources::loadFont("FifteenNarrow.ttf", 10);

  if (!ok) {
    printf("Could not load assets\n");
  }

  player = new Player();

  /*
  walker = new Sprite("walk_test.png");
  walker->spritesheet(32, 32);
  walker->addAnimation("idle", { 35, 36, 37, 38, 39 });
  walker->playAnimation("idle");

  walker->x = 35 * 16;
  walker->y = 10 * 16;*/

  info = new Info(33 * 16, 10 * 16);

  battery = new Battery();

  // position battery at bottom left of the screen
  battery->sprite->x = camera.width - (battery->width + 4);
  battery->sprite->y = camera.height - (battery->height + 4);

  map = new Tilemap();
  map->loadTileset("tileset.png");
  map->loadBackground("assets/lvl/level_background.csv");
  map->loadForeground("assets/lvl/level_foreground.csv");
  map->loadCollision("assets/lvl/level_collision.csv");

  Core::clear = SDL_Color{.r = 10, .g = 10, .b = 13, .a = 255};

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
    }
  }

  Tilemap::collide(player->sprite, map);
  info->showText = Sprite::isOverlapping(player->sprite->rect(), info->sprite->rect());

  if (action.justDown()) {
    battery->capacity += 0.05;

    if (battery->capacity > 1) {
      battery->capacity = 1;
    }
  }

  player->tick(dt);
  info->tick(dt);

  camera.update();
}

void Game::render(SDL_Renderer* renderer) {
  SDL_Point cam = camera.point();

  map->renderBackground(renderer, camera);
  info->render(renderer, cam);

  player->render(renderer, cam);
  map->renderForeground(renderer, camera);

  // HUD
  battery->render(renderer);
}
