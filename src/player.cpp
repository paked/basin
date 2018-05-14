#include <player.hpp>

#include <e/collision.hpp>
#include <e/core.hpp>

void Player::start() {
  sprite = new Spritesheet("player.png", 10, 19);

  sprite->addAnimation("idle_hori", { 0, 1 });
  sprite->addAnimation("walk_hori", { 2, 3 });
  sprite->addAnimation("idle_down", { 4, 5 });
  sprite->addAnimation("walk_down", { 6, 7 });
  sprite->addAnimation("idle_up", { 8, 9 });
  sprite->addAnimation("walk_up", { 10, 11 });

  sprite->playAnimation("idle_down");

  textEquip = new Text("Pick up with E");
  textInsert = new Text("Insert with E");
  textPower = new Text("Power on with space");

  battery = new Battery();
  torch = new Torch();

  sprite->maxVelocity = Point(25, 25);
  sprite->drag = Point(0.95, 0.95);
  /*sprite->x = (10 * 16)*Core::scale - sprite->width/2;
  sprite->y = (2 * 16)*Core::scale;*/
  sprite->x = (17 * 16)*Core::scale - sprite->width/2;
  sprite->y = (18 * 16)*Core::scale;

  reg(sprite);
  reg(battery);
  reg(torch);
}

void Player::tick(float dt) {
  Entity::tick(dt);

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

  lastDroppedItem = nullptr;
  justDroppedItem = false;

  /*
  if (busy) {
    // Stop player from moving, picking things up, etc. while interacting with a panel or whatever.
    // Not entirely sure what the ramifications of this will be...
    return;
  }*/

  if (currentMovement == MOVE_LEFT) {
    sprite->acceleration.x = -acceleration;
  } else if (currentMovement == MOVE_RIGHT) {
    sprite->acceleration.x = acceleration;
  } else if (currentMovement == MOVE_UP) {
    sprite->acceleration.y = -acceleration;
  } else if (currentMovement == MOVE_DOWN) {
    sprite->acceleration.y = acceleration;
  }

  if (hasItem && equip.justDown() && !justGotItem) {
    hasItem = false;
    justDroppedItem = true;
    item->visible = true;

    battery->unattach();

    lastDroppedItem = item;
    item = nullptr;
  }

  if (hasItem && item->type == Collectable::TORCH && use.justDown()) {
    torch->on = !torch->on;
  }

  if (hasItem && item->type == Collectable::TORCH && battery->capacity < 0) {
    torch->on = false;
  }

  if (hasItem) {
    positionItem();
  }

  justGotItem = false;

  // send render commands

  if (doPrompt && promptOn) {
    SDL_Rect rect = promptText->rect;

    SDL_Rect dst = {
      .x = promptPosition.x - rect.w,
      .y = promptPosition.y - rect.h,
      .w = rect.w*2,
      .h = rect.h*2
    };

    RenderJob j;
    j.depth = DEPTH_UI;
    j.tex = promptText->texture;
    j.src = promptText->rect;
    j.dst = dst;

    scene->renderer->queue.push(j);
  }
}

void Player::postTick() {
  if (!doPrompt) {
    return;
  }

  if (promptClear) {
    doPrompt = false;
    promptText = nullptr;

    return;
  }

  if (SDL_GetTicks() > promptNextToggle) {
    promptOn = !promptOn;

    promptNextToggle = SDL_GetTicks() + promptToggleFrequency;
  }

  promptClear = true;
}

bool Player::equipMeMaybe(Collectable* c) {
  if (hasItem || justDroppedItem || busy) {
    return false;
  }

  if (!Collision::isOverlapping(sprite, c->sprite->rect())) {
    return false;
  }

  if (c->shouldPromptEquip) {
    proposePrompt(textEquip);
  }

  if (!equip.justDown()) {
    // The equip button was not pressed, no chance of pick up.
    return false;
  }

  hasItem = true;
  justGotItem = true;

  item = c;

  // after we pick up an item once, we don't need to be in-your-face about
  // suggesting to pick it up again
  item->shouldPromptEquip = false;

  printf("picking up %s\n", Collectable::key(c->type).c_str());

  battery->attach(c->type);

  return true;
}

// make the item look like it is being carried around by the player
void Player::positionItem() {
  item->sprite->y = sprite->y + 32;
  item->localDepth = DEPTH_ABOVE;
  item->visible = true;
  item->sprite->flip = false;

  item->sprite->angle = 0;

  if (eyeLine == Torch::LEFT) {
    item->sprite->flip = true;
    item->sprite->x = sprite->x - 16;
  } else if (eyeLine == Torch::RIGHT) {
    item->sprite->x = sprite->x + 20;
  } else if (eyeLine == Torch::UP) {
    item->sprite->x = sprite->x + 4;
    item->localDepth = DEPTH_BELOW;
  } else if (eyeLine == Torch::DOWN) {
    item->sprite->angle = 90;
    item->sprite->y += 4;
    item->sprite->x = sprite->x + 4;
  }
}

void Player::proposePrompt(Text* text) {
  if (promptText == text) {
    promptClear = false;

    return;
  }

  promptText = text;
  doPrompt = true;
  promptOn = true;
  promptClear = false;
  promptNextToggle = SDL_GetTicks() + promptToggleFrequency;
}
