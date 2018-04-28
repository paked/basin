#include <player.hpp>

#include <e/core.hpp>

Player::Player() {
  sprite = new Sprite("player.png");

  sprite->spritesheet(10, 19);
  sprite->addAnimation("idle_hori", { 0, 1 });
  sprite->addAnimation("walk_hori", { 2, 3 });
  sprite->addAnimation("idle_down", { 4, 5 });
  sprite->addAnimation("walk_down", { 6, 7 });
  sprite->addAnimation("idle_up", { 8, 9 });
  sprite->addAnimation("walk_up", { 10, 11 });

  sprite->playAnimation("idle_down");

  equipPrompt = new Text("E to equip");

  battery = new Battery();
  torch = new Torch();

  sprite->maxVelocity = Point(25, 25);
  sprite->drag = Point(0.95, 0.95);
  sprite->x = (9 * 16)*Core::scale - sprite->width/2;
  sprite->y = (1 * 16)*Core::scale;
}

void Player::tick(float dt) {
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

  if (currentMovement == MOVE_LEFT) {
    sprite->acceleration.x = -acceleration;
  } else if (currentMovement == MOVE_RIGHT) {
    sprite->acceleration.x = acceleration;
  } else if (currentMovement == MOVE_UP) {
    sprite->acceleration.y = -acceleration;
  } else if (currentMovement == MOVE_DOWN) {
    sprite->acceleration.y = acceleration;
  }

  justDroppedItem = false;
  if (hasItem && equip.justDown() && !justGotItem) {
    hasItem = false;
    justDroppedItem = true;
    item->visible = true;

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

  if (torch->darkness > 0.1) {
    torch->pre();

    if (hasItem && item->type == Collectable::TORCH) {
      torch->beamIn(eyeLine);
    }

    torch->post();
  }
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
    SDL_Rect rect = equipPrompt->rect;

    SDL_Rect dst = {
      .x = sprite->x - (rect.w/2 - sprite->rect().w/2),
      .y = sprite->y - (rect.h/2 - sprite->rect().h/2) - 64,
      .w = rect.w,
      .h = rect.h
    };

    dst.x -= camera.x;
    dst.y -= camera.y;

    SDL_RenderCopy(renderer, equipPrompt->texture, NULL, &dst);

    // BAD CODE. relies on rendering being done last in the frame in order to
    // reset the showEquipPrompt flag.
    showEquipPrompt = false;
  }

  if (torch->darkness > 0.1) {
    torch->render(renderer);
  }
}

bool Player::equipMeMaybe(Collectable* c) {
  if (hasItem || justDroppedItem) {
    return false;
  }

  if (!Sprite::isOverlapping(sprite->rect(), c->sprite->rect())) {
    return false;
  }

  showEquipPrompt = true;

  if (!equip.justDown()) {
    // The equip button was not pressed, no chance of pick up.
    return false;
  }

  hasItem = true;

  item = c;

  printf("picking up %s\n", Collectable::key(c->type).c_str());

  justGotItem = true;

  battery->attach(c->type);

  return true;
}

// make the item look like it is being carried around by the player
void Player::positionItem() {
  item->sprite->y = sprite->y + 24;
  item->visible = true;
  item->sprite->flip = false;

  if (eyeLine == Torch::LEFT) {
    item->sprite->flip = true;
    item->sprite->x = sprite->x - 16;
  } else if (eyeLine == Torch::RIGHT) {
    item->sprite->x = sprite->x + 20;
  } else if (eyeLine == Torch::UP) {
    item->sprite->x = sprite->x + 4;
    item->visible = false;
  } else if (eyeLine == Torch::DOWN) {
    item->sprite->y += 4;
    item->sprite->x = sprite->x + 4;
  }
}
