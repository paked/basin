#include <e/camera.hpp>

void Camera::update() {
  if (!follow) {
    return;
  }

  x = (follow->x - follow->width/2) - width/2;
  y = (follow->y - follow->height/2) - height/2;
}

bool Camera::withinViewport(SDL_Rect rect) {
  SDL_Rect me = SDL_Rect{
    .x = x - 32,
    .y = y - 32,
    .w = width + 64,
    .h = height + 64
  };

  return SDL_HasIntersection(&rect, &me);
}

SDL_Point Camera::point() {
  return SDL_Point{
    .x = x,
    .y = y
  };
}
