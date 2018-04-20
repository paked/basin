#include <game.hpp>

#include <stdio.h>

#include <e/resources.hpp>

void Game::load() {
  bool ok = Resources::load("character.png");
  ok |= Resources::load("wall.png");

  if (!ok) {
    printf("Could not load assets\n");
  }

  player = new Sprite("character.png", 400-8, 400-8, 64, 64);
  wall = new Sprite("wall.png", 400, 20);
}

void Game::collide(Sprite* first, Sprite* second) {
  SDL_Rect b = second->rect();
  SDL_Rect res;

  // First case we check against next y position
  {
    SDL_Rect a = first->rect();
    a.y += (int)first->nextPositionDelta.y;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.y < 0) ? 1 : -1;
      first->nextPositionDelta.y += (int)res.h * mod;
    }
  }

  // Then we check against the next x position
  {
    SDL_Rect a = first->rect();
    a.x += (int)first->nextPositionDelta.x;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.x < 0) ? 1 : -1;
      first->nextPositionDelta.x += (int)res.w * mod;
    }
  }
}

void Game::tick(float dt) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = true;
    } else if (event.type == SDL_KEYDOWN) {
      if(event.key.keysym.scancode == SDL_SCANCODE_A) {
        playerMoveLeft = true;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_D) {
        playerMoveRight = true;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_W) {
        playerMoveUp = true;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_S) {
        playerMoveDown = true;
      }
    } else if (event.type == SDL_KEYUP) {
      if(event.key.keysym.scancode == SDL_SCANCODE_A) {
        playerMoveLeft = false;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_D) {
        playerMoveRight = false;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_W) {
        playerMoveUp = false;
      } else if(event.key.keysym.scancode == SDL_SCANCODE_S) {
        playerMoveDown = false;
      }
    }
  }

  collide(player, wall);

  player->acceleration.x = 0;
  player->acceleration.y = 0;

  if (playerMoveRight) {
    player->acceleration.x = 80;
  }

  if (playerMoveLeft) {
    player->acceleration.x = -80;
  }

  if (playerMoveUp) {
    player->acceleration.y = -80;
  }

  if (playerMoveDown) {
    player->acceleration.y = 80;
  }

  player->tick(dt);
  wall->tick(dt);
}

void Game::render(SDL_Renderer* renderer) {
  wall->render(renderer);

  player->render(renderer);
}
