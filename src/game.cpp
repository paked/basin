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
  ok |= Resources::load("slime.png");
  ok |= Resources::load("collectables.png");
  ok |= Resources::load("sliding_door.png");
  ok |= Resources::load("battery_attachments.png");
  ok |= Resources::loadFont("Cave-Story.ttf", 15);

  if (!ok) {
    printf("Could not load assets\n");
  }

  player = new Player();
  enemy = new Enemy(33 * 16, 10 * 16);
  chainsaw = new Collectable(37 * 16, 8 * 16, Collectable::Type::KEY);
  slidingDoor = new SlidingDoor(33 * 16, 11 * 16);

  info = new Info(33 * 16, 10 * 16);

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
  Sprite::collide(player->sprite, slidingDoor->rect());
  info->showText = Sprite::isOverlapping(player->sprite->rect(), info->sprite->rect());

  player->equipMeMaybe(Collectable::key(chainsaw->type), chainsaw);

  player->tick(dt);
  slidingDoor->tick(dt);
  enemy->tick(dt);
  info->tick(dt);
  chainsaw->tick(dt);

  camera.update();
}

void Game::render(SDL_Renderer* renderer) {
  SDL_Point cam = camera.point();

  map->renderBackground(renderer, camera);

  chainsaw->render(renderer, cam);
  info->render(renderer, cam);
  enemy->render(renderer, cam);

  player->render(renderer, cam);
  slidingDoor->render(renderer, cam);

  map->renderForeground(renderer, camera);
  player->renderForeground(renderer, camera);
}
