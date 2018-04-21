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

  mapTexture = Resources::get("tileset.png", &mapTextureWidth, &mapTextureHeight);

  CSV levelCSV("assets/level.csv");
  mapData = levelCSV.getData();

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

  for (int y = 0; y < mapData.size(); y++) {
    auto row = mapData[y];

    for (int x = 0; x < row.size(); x++) {
      auto tile = row[x];

      int xIndex;
      int yIndex;

      if (tile == "33") {
        xIndex = 1;
        yIndex = 2;
      } else {
        continue;
      }

      SDL_Rect src = {
        .x = xIndex * mapTextureTileSize,
        .y = yIndex * mapTextureTileSize,
        .w = 16,
        .h = 16
      };

      SDL_Rect dst = {
        .x = x * mapTextureTileSize,
        .y = y * mapTextureTileSize,
        .w = 16,
        .h = 16
      };

      if (!camera.withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }

      dst.x -= cam.x;
      dst.y -= cam.y;

      SDL_RenderCopy(renderer, mapTexture, &src, &dst);
    }
  }

  wall->render(renderer, cam);

  player->render(renderer, cam);
}
