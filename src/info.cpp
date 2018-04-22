#include <info.hpp>

#include <stdio.h>
#include <cmath>

#include <e/resources.hpp>
#include <e/core.hpp>

#define DEGREES_TO_RADIANS(degrees) ((degrees) * 3.14159265359 / 180.0)

Info::Info(float x, float y) {
  sprite = new Sprite("info.png", x, y);

  starting = Point(x, y);

  text = new Text("go that way");
}

void Info::tick(float dt) {
  sprite->y = (int) starting.y + std::sin(DEGREES_TO_RADIANS(SDL_GetTicks()/1.3))*1;

  sprite->tick(dt);
}

void Info::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);

  if (!showText) {
    return;
  }

  SDL_Rect dst = {
    .x = (int)(starting.x - text->rect.w/2 - cam.x),
    .y = (int)(starting.y - text->rect.h/2 - cam.y - 16),
    .w = text->rect.w,
    .h = text->rect.h,
  };

  SDL_RenderCopy(renderer, text->texture, NULL, &dst);
}
