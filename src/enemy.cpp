#include <enemy.hpp>

/*
Enemy::Enemy(int x, int y) {
  sprite = new Sprite("slime.png");
  sprite->spritesheet(16, 16);

  sprite->addAnimation("walk_hori", { 0, 1});
  sprite->addAnimation("walk_up", { 2, 1});

  sprite->playAnimation("walk_hori");

  sprite->x = x;
  sprite->y = y;

  sprite->drag.x = 0.15;
}

void Enemy::tick(float dt) {
  if (sprite->velocity.x < 0.1 && move) {
    moveAt = SDL_GetTicks() + moveDelay;

    move = false;
  }

  if (!move && SDL_GetTicks() > moveAt) {
    sprite->velocity.x = 10;

    move = true;
  }

  sprite->tick(dt);
}

void Enemy::render(SDL_Renderer* renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}*/
