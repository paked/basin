#include <player.hpp>

Player::Player() {
  sprite = new Sprite("player.png");

  sprite->maxVelocity = Point(10, 10);

  sprite->spritesheet(10, 19);
  sprite->addAnimation("idle_hori", { 0, 1 });
  sprite->addAnimation("walk_hori", { 2, 3 });
  sprite->addAnimation("idle_down", { 4, 5 });
  sprite->addAnimation("walk_down", { 6, 7 });
  sprite->addAnimation("idle_up", { 8, 9 });
  sprite->addAnimation("walk_up", { 10, 11 });
  sprite->x = 33 * 16;
  sprite->y = 8 * 16;

  sprite->playAnimation("idle");
}

void Player::tick(float dt) {
  if (moveLeft.justDown()) {
    currentMovement = MOVE_LEFT;

    sprite->flip = true;
    sprite->playAnimation("walk_hori");
  }

  if (moveRight.justDown()) {
    currentMovement = MOVE_RIGHT;

    sprite->flip = false;
    sprite->playAnimation("walk_hori");
  }

  if (moveUp.justDown()) {
    currentMovement = MOVE_UP;
    sprite->playAnimation("walk_up");
  }

  if (moveDown.justDown()) {
    currentMovement = MOVE_DOWN;
    sprite->playAnimation("walk_down");
  }

  if (moveLeft.justUp() && currentMovement == MOVE_LEFT) {
    currentMovement = IDLE;

    sprite->playAnimation("idle_hori");
  }

  if (moveRight.justUp() && currentMovement == MOVE_RIGHT) {
    currentMovement = IDLE;

    sprite->playAnimation("idle_hori");
  }

  if (moveUp.justUp() && currentMovement == MOVE_UP) {
    currentMovement = IDLE;

    sprite->playAnimation("idle_up");
  }

  if (moveDown.justUp() && currentMovement == MOVE_DOWN) {
    currentMovement = IDLE;

    sprite->playAnimation("idle_down");
  }


  sprite->acceleration.x = 0;
  sprite->acceleration.y = 0;

  int accel = 40;

  if (currentMovement == MOVE_LEFT) {
    sprite->acceleration.x = -accel;
  } else if (currentMovement == MOVE_RIGHT) {
    sprite->acceleration.x = accel;
  } else if (currentMovement == MOVE_UP) {
    sprite->acceleration.y = -accel;
  } else if (currentMovement == MOVE_DOWN) {
    sprite->acceleration.y = accel;
  }

  sprite->tick(dt);
}

void Player::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}
