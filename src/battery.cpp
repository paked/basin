#include <battery.hpp>

#include <e/resources.hpp>

Battery::Battery(int x, int y) {
  sprite = new Sprite("battery.png");
  sprite->spritesheet(18, 22);

  width = sprite->width;
  height = sprite->height;
}

void Battery::render(SDL_Renderer *renderer) {
  SDL_Rect src = sprite->getFrame(0);
  SDL_Texture* texture = sprite->texture;

  int x = sprite->x;
  int y = sprite->y;

  SDL_Rect dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);

  src = sprite->getFrame(1);

  dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);

  int battSize = (int) (height * capacity);
  int offset = height - battSize;

  src = sprite->getFrame(2);

  src.y += offset;
  src.h = battSize;

  dst = {
    .x = x,
    .y = y + offset,
    .w = width,
    .h = battSize
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);
}
