#include <e/camera.hpp>

void Camera::update() {
  if (!follow) {
    return;
  }

  x = follow->x - (width - follow->width)/2;
  y = follow->y - (height - follow->height)/2;
}

bool Camera::withinViewport(SDL_Rect rect) {
  SDL_Rect me = viewport(32);

  return SDL_HasIntersection(&rect, &me);
}

SDL_Point Camera::point() {
  return SDL_Point{
    .x = x,
    .y = y
  };
}

SDL_Rect Camera::viewport(int buffer) {
  return SDL_Rect{
    .x = x - buffer,
    .y = y - buffer,
    .w = width + buffer*2,
    .h = height + buffer*2
  };
}
