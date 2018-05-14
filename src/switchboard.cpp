#include <switchboard.hpp>

#include <stdio.h>

#include <e/input.hpp>
#include <e/core.hpp>

#include <config.hpp>

void Switchboard::start() {
  localDepth = DEPTH_UI;
  backboard = new Sprite("switchboard_gui.png");
  backboard->localDepth = DEPTH_BELOW;

  backboard->x = SCREEN_WIDTH/2 - backboard->width/2;
  backboard->y = SCREEN_HEIGHT/2 - backboard->height/2;
  backboard->hud = true;

  inPositive = new Jumper(true, { backboard->x + (2 * 16) * Core::scale, backboard->y + (6 * 16) * Core::scale });
  inNegative = new Jumper(false, { backboard->x + (6 * 16) * Core::scale, backboard->y + (6 * 16) * Core::scale });

  outPositive = new Jumper(true, { backboard->x + (2 * 16) * Core::scale, backboard->y + (2 * 16) * Core::scale });
  outNegative = new Jumper(false, { backboard->x + (6 * 16) * Core::scale, backboard->y + (2 * 16) * Core::scale });

  inPositive->sprite->x = 20 * Core::scale;
  inPositive->sprite->y = 150 * Core::scale;
  inNegative->sprite->x = 170 * Core::scale;
  inNegative->sprite->y = 150 * Core::scale;

  outPositive->sprite->x = 20 * Core::scale;
  outPositive->sprite->y = 30 * Core::scale;
  outNegative->sprite->x = 170 * Core::scale;
  outNegative->sprite->y = 30 * Core::scale;

  reg(backboard);
  reg(inPositive->sprite);
  reg(inNegative->sprite);
  reg(outPositive->sprite);
  reg(outNegative->sprite);
}

void Switchboard::tick(float dt) {
  SDL_Point origin = { backboard->x, backboard->y };

  SDL_Point point = { Input::mouseX, Input::mouseY };

  std::vector<Jumper*> grabbables = { inPositive, inNegative, outPositive, outNegative };
  for (auto grabbable : grabbables) {
    SDL_Rect rect = grabbable->sprite->rect();

    if (Input::mouseJustDown() && SDL_PointInRect(&point, &rect)) {
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
