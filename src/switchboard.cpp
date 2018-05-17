#include <switchboard.hpp>

#include <stdio.h>

#include <e/input.hpp>
#include <e/point.hpp>
#include <e/rect.hpp>
#include <e/collision.hpp>

#include <config.hpp>

void Switchboard::start() {
  localDepth = DEPTH_UI;
  backboard = new Sprite("switchboard_gui.png");
  backboard->localDepth = DEPTH_BELOW;

  backboard->x = SCREEN_WIDTH/2 - backboard->width/2;
  backboard->y = SCREEN_HEIGHT/2 - backboard->height/2;
  backboard->hud = true;

  inPositive = new Jumper(true, { backboard->x + (2 * 16), backboard->y + (6 * 16) });
  inNegative = new Jumper(false, { backboard->x + (6 * 16), backboard->y + (6 * 16) });

  outPositive = new Jumper(true, { backboard->x + (2 * 16), backboard->y + (2 * 16) });
  outNegative = new Jumper(false, { backboard->x + (6 * 16), backboard->y + (2 * 16) });

  inPositive->sprite->x = 20;
  inPositive->sprite->y = 150;
  inNegative->sprite->x = 170;
  inNegative->sprite->y = 150;

  outPositive->sprite->x = 20;
  outPositive->sprite->y = 30;
  outNegative->sprite->x = 170;
  outNegative->sprite->y = 30;

  reg(backboard);
  reg(inPositive->sprite);
  reg(inNegative->sprite);
  reg(outPositive->sprite);
  reg(outNegative->sprite);
}

void Switchboard::tick(float dt) {
  Point origin = { backboard->x, backboard->y };

  Point point = { Input::mouseX, Input::mouseY };

  std::vector<Jumper*> grabbables = { inPositive, inNegative, outPositive, outNegative };
  for (auto grabbable : grabbables) {
    Rect rect = grabbable->sprite->rect();

    if (Input::mouseJustDown() && Collision::isOverlapping(point, rect)) {
      pinned = grabbable;

      offset = SDL_Point {point.x - rect.x, point.y - rect.y};
      break;
    }
  }

  if (pinned && Input::mouseJustUp()) {
    pinned = nullptr;
  }

  if (pinned) {
    pinned->sprite->x = point.x - offset.x;
    pinned->sprite->y = point.y - offset.y;
  }

  Entity::tick(dt);
}

bool Switchboard::continuous() {
  std::vector<Jumper*> grabbables = { inPositive, inNegative, outPositive, outNegative };

  bool ok = true;
  for (auto grabbable : grabbables) {
    if (!grabbable->in()) {
      ok = false;

      break;
    }
  }

  return ok;
}
