#include <info.hpp>

#include <stdio.h>
#include <cmath>

#include <e/resources.hpp>
#include <e/core.hpp>

#define DEGREES_TO_RADIANS(degrees) ((degrees) * 3.14159265359 / 180.0)

Info::Info(float x, float y, std::string msg) {
  sprite = new Sprite("info.png", x, y);
  sprite->spritesheet(8, 8);

  sprite->addAnimation("unread", { 0 });
  sprite->addAnimation("read", { 1 });

  starting = Point(x, y);

  text = new Text(msg);

  sprite->playAnimation("unread");
}

void Info::tick(float dt) {
  if (!read && showText) {
    read = true;

    sprite->playAnimation("read");
  }

  sprite->y = (int) starting.y + std::sin(DEGREES_TO_RADIANS(SDL_GetTicks()/1.3))*1;

  sprite->tick(dt);
}

void Info::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);

  if (!showText) {
    return;
  }

  SDL_Rect trect = text->rect;
  trect.w /= 2;
  trect.h /= 2;

  SDL_Rect dst = {
    .x = (int)(starting.x - trect.w/2 - cam.x),
    .y = (int)(starting.y - trect.h/2 - cam.y - 16),
    .w = trect.w,
    .h = trect.h,
  };

  SDL_RenderCopy(renderer, text->texture, NULL, &dst);
}
