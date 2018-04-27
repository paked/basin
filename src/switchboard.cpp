#include <switchboard.hpp>

#include <stdio.h>

#include <e/input.hpp>

Switchboard::Switchboard(int x, int y) {
  backboard = new Sprite("switchboard_gui.png");
  overlay = new Sprite("switchboard_gui_overlay.png");
  overlay->spritesheet(64, 64);

  backboard->x = overlay->x = x - backboard->width/2;
  backboard->y = overlay->y = y - backboard->height/2;
  backboard->hud = overlay->hud = true;

  inPositive = new Jumper(true, { backboard->x + 2 * 16, backboard->y + 6 * 16 });
  inNegative = new Jumper(false, { backboard->x + 6 * 16, backboard->y + 6 * 16 });

  outPositive = new Jumper(true, { backboard->x + 2 * 16, backboard->y + 2 * 16 });
  outNegative = new Jumper(false, { backboard->x + 6 * 16, backboard->y + 2 * 16 });

  inPositive->sprite->x = 20;
  inPositive->sprite->y = 150;
  inNegative->sprite->x = 170;
  inNegative->sprite->y = 150;

  outPositive->sprite->x = 20;
  outPositive->sprite->y = 30;
  outNegative->sprite->x = 170;
  outNegative->sprite->y = 30;
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

  backboard->tick(dt);
}

void Switchboard::render(SDL_Renderer* renderer, SDL_Point cam) {
  backboard->render(renderer, cam);

  // TODO: clean this up
  SDL_Rect og = overlay->rect();

  outPositive->sprite->render(renderer, cam);
  if (!outPositive->dirty()) {
    overlay->x = backboard->x;
    overlay->y = backboard->y;
    overlay->renderFrame(0, renderer, cam);
  }

  outNegative->sprite->render(renderer, cam);
  if (!outNegative->dirty()) {
    overlay->x = backboard->x + backboard->width/2;
    overlay->y = backboard->y;

    overlay->renderFrame(1, renderer, cam);
  }

  inPositive->sprite->render(renderer, cam);
  if (!inPositive->dirty()) {
    overlay->x = backboard->x;
    overlay->y = backboard->y + backboard->height/2;

    overlay->renderFrame(2, renderer, cam);
  }

  inNegative->sprite->render(renderer, cam);
  if (!inNegative->dirty()) {
    overlay->x = backboard->x + backboard->width/2;
    overlay->y = backboard->y + backboard->height/2;

    overlay->renderFrame(3, renderer, cam);
  }

  overlay->x = og.x;
  overlay->y = og.y;
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
