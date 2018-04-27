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
  sprite->y = 2 * 16;

  sprite->playAnimation("idle");

  equipPrompt = new Text("E to equip");

  battery = new Battery();
  torch = new Torch();
}

void Player::tick(float dt) {
  torch->pre();

  if (moveLeft.justDown()) {
    currentMovement = MOVE_LEFT;
    eyeLine = Torch::LEFT;

    sprite->flip = true;
    sprite->playAnimation("walk_hori");
  }

  if (moveRight.justDown()) {
    currentMovement = MOVE_RIGHT;
    eyeLine = Torch::RIGHT;

    sprite->flip = false;
    sprite->playAnimation("walk_hori");
  }

  if (moveUp.justDown()) {
    currentMovement = MOVE_UP;
    eyeLine = Torch::UP;

    sprite->playAnimation("walk_up");
  }

  if (moveDown.justDown()) {
    currentMovement = MOVE_DOWN;
    eyeLine = Torch::DOWN;

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
    justDroppedItem = true;

    battery->unattach();

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

  if (hasItem) {
    positionItem();
  }

  sprite->tick(dt);
  battery->tick(dt);

  justGotItem = false;

  torch->beamIn(eyeLine);

  torch->post();
}

void Player::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}

void Player::renderForeground(SDL_Renderer* renderer, Camera camera) {
  // position battery at bottom left of the screen
  battery->sprite->x = camera.width - (battery->width + 4);
  battery->sprite->y = camera.height - (battery->height + 4);

  battery->render(renderer, camera.point());

  if (showEquipPrompt) {
    SDL_Rect dst = {
      .x = (int)(sprite->x - equipPrompt->rect.w/2 - camera.x),
      .y = (int)(sprite->y - equipPrompt->rect.h/2 - camera.y - 16),
      .w = equipPrompt->rect.w,
      .h = equipPrompt->rect.h,
    };

    SDL_RenderCopy(renderer, equipPrompt->texture, NULL, &dst);
  }

  torch->render(renderer);
}

bool Player::equipMeMaybe(Collectable* c) {
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

  item = c;
  // item->active = false;

  printf("picking up %s\n", Collectable::key(c->type).c_str());

  justGotItem = true;

  battery->attach(c->type);

  return true;
}

// super jank conditional to make the item follow along with the body in  a way
// which looks like it is being carried by hand
void Player::positionItem() {
  std::string name = sprite->currentAnimationName;

  item->sprite->flip = false;

  item->sprite->y = sprite->y + 6;
  if (name == "idle_hori") {
    if (!sprite->flip) {
      item->sprite->x = sprite->x + 10;
    } else {
      item->sprite->x = sprite->x - 8;
      item->sprite->flip = true;
    }
  } else if (name == "walk_hori") {
    if (!sprite->flip) {
      item->sprite->x = sprite->x + 11;
    } else {
      item->sprite->x = sprite->x - 9;
      item->sprite->flip = true;
    }
  } else if (name == "idle_down" || name == "walk_down") {
    item->sprite->x = sprite->x + 10;
  } else if (name == "walk_up" || name == "idle_up") {
    item->sprite->flip = true;
    item->sprite->x = sprite->x - 8;
  }
}
