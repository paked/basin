#include <battery.hpp>

#include <e/resources.hpp>

Battery::Battery(int x, int y) : x(x), y(y) {
  int w;
  int h;

  texture = Resources::get("battery.png", &w, &h);

  height = h;
  width = w/frames;
}

void Battery::render(SDL_Renderer *renderer) {
  SDL_Rect src = {
    .x = 1*width,
    .y = 0,
    .w = width,
    .h = height
  };

  SDL_Rect dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);

  int offset = (int) (height*capacity);

  src = {
    .x = 2*width,
    .y = height - offset,
    .w = width,
    .h = offset
  };

  dst = {
    .x = x,
    .y = y + height - offset,
    .w = width,
    .h = offset
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);

  src = {
    .x = 0*width,
    .y = 0,
    .w = width,
    .h = height
  };

  dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  SDL_RenderCopy(renderer, texture, &src, &dst);
}
