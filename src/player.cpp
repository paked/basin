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

  equipPrompt = new Text("E to equip");

  battery = new Battery();
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

  justDroppedItem = false;
  if (hasItem && equip.justDown() && !justGotItem) {
    hasItem = false;
    hasChainsaw = false;

    justDroppedItem = true;

    item->active = true;
    item->sprite->x = sprite->x;
    item->sprite->y = sprite->y;

    item = nullptr;
  }

  if (use.justDown()) {
    itemOn = !itemOn;
  }

  if (hasItem && itemOn) {
    battery->capacity -= 0.05 * dt;
  }

  if (battery->capacity < 0) {
    itemOn = false;
  }

  sprite->tick(dt);

  justGotItem = false;
}

void Player::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}

void Player::renderForeground(SDL_Renderer* renderer, Camera camera) {
  // position battery at bottom left of the screen
  battery->sprite->x = camera.width - (battery->width + 4);
  battery->sprite->y = camera.height - (battery->height + 4);

  battery->render(renderer);

  if (!showEquipPrompt) {
    return;
  }
 
  SDL_Rect dst = {
    .x = (int)(sprite->x - equipPrompt->rect.w/2 - camera.x),
    .y = (int)(sprite->y - equipPrompt->rect.h/2 - camera.y - 16),
    .w = equipPrompt->rect.w,
    .h = equipPrompt->rect.h,
  };

  SDL_RenderCopy(renderer, equipPrompt->texture, NULL, &dst);
}

bool Player::equipMeMaybe(std::string type, Collectable* c) {
  if (hasItem || justDroppedItem) {
    showEquipPrompt = false;
    return false;
  }

  bool can = Sprite::isOverlapping(sprite->rect(), c->sprite->rect());

  if (can) {
    showEquipPrompt = true;
  } else {
    showEquipPrompt = false;

    return false;
  }

  if (!equip.justDown()) {
    return false;
  }

  hasItem = true;
  hasChainsaw = true;

  item = c;
  item->active = false;

  printf("picking up %s\n", type.c_str());

  justGotItem = true;

  return true;
}
