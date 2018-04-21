#include <game.hpp>

#include <stdio.h>

#include <e/csv.hpp>
#include <e/resources.hpp>

void Game::load() {
  bool ok = Resources::load("player.png");
  ok |= Resources::load("wall.png");
  ok |= Resources::load("tileset.png");

  if (!ok) {
    printf("Could not load assets\n");
  }

  player = new Sprite("player.png", 200-8, 200-8);
  wall = new Sprite("wall.png", 0, 0);

  map = new Tilemap();
  map->load("assets/lvl/level_base.csv", "tileset.png");

  camera.follow = player;
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

  Sprite::collide(player, wall);

  player->acceleration.x = 0;
  player->acceleration.y = 0;

  if (moveLeft.down()) {
    player->acceleration.x = -80;
  }

  if (moveRight.down()) {
    player->acceleration.x = 80;
  }

  if (moveUp.down()) {
    player->acceleration.y = -80;
  }

  if (moveDown.down()) {
    player->acceleration.y = 80;
  }

  player->tick(dt);
  wall->tick(dt);

  camera.update();
}

void Game::render(SDL_Renderer* renderer) {
  SDL_Point cam = camera.point();

  map->render(renderer, cam);
  wall->render(renderer, cam);
  player->render(renderer, cam);
}
