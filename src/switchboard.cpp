#include <switchboard.hpp>

#include <stdio.h>

#include <e/input.hpp>
#include <e/core.hpp>

#include <config.hpp>

void Switchboard::start() {
  localDepth = DEPTH_UI;
  backboard = new Sprite("switchboard_gui.png");
  overlay = new Sprite("switchboard_gui_overlay.png");
  overlay->spritesheet(64, 64);

  backboard->x = overlay->x = SCREEN_WIDTH/2 - backboard->width/2;
  backboard->y = overlay->y = SCREEN_HEIGHT/2 - backboard->height/2;
  backboard->hud = overlay->hud = true;

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

  // Send render commands
  backboard->job(scene, getDepth() + DEPTH_BELOW);

  inPositive->sprite->job(scene, getDepth());
  outPositive->sprite->job(scene, getDepth());
  inNegative->sprite->job(scene, getDepth());
  outNegative->sprite->job(scene, getDepth());
}

void Switchboard::renderOverlay(SDL_Renderer* renderer, SDL_Point cam) {
  backboard->render(renderer, cam);

  inPositive->render(renderer, cam);
  outPositive->render(renderer, cam);
  inNegative->render(renderer, cam);
  outNegative->render(renderer, cam);

  /*
  // TODO: re add in the insert thing
  SDL_Rect og = overlay->rect();

  outPositive->render(renderer, cam);
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
  overlay->y = og.y;*/
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
