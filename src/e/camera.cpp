#include <e/camera.hpp>

#include <e/sprite.hpp>

#include <e/math_util.hpp>

void Camera::tick(float dt) {
  if (follow) {
    realX = follow->x - (width - follow->width)/2;
    realY = follow->y - (height - follow->height)/2;
  }

  x = realX;
  y = realY;

  float shake = shakeTrauma*shakeTrauma;

  if (SDL_GetTicks() > shakeStopTime) {
    shakeTrauma -= 0.2 * dt;

    if (shakeTrauma < 0) {
      shakeTrauma = 0;
    }
  }

  x += shakeMax * shake * MathUtil::randN1P1();
  y += shakeMax * shake * MathUtil::randN1P1();
}

void Camera::shake(int duration, float st) {
  shakeTrauma = st;
  shakeStopTime = SDL_GetTicks() + duration;
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
