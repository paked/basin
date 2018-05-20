#include <e/camera.hpp>

#include <e/sprite.hpp>
#include <e/collision.hpp>
#include <e/math_util.hpp>
#include <e/point.hpp>

void Camera::tick(float dt) {
  if (follow) {
    Rect r = follow->rect();
    realX = r.x - (getWidth() - r.w)/2;
    realY = r.y - (getHeight() - r.h)/2;
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

SDL_Rect Camera::toView(Rect rect, bool global) {
  if (!global) {
    rect.x -= x;
    rect.y -= y;
  }

  rect.x *= zoom;
  rect.y *= zoom;
  rect.w *= zoom;
  rect.h *= zoom;

  return Rect::toSDL(rect);
}

bool Camera::withinViewport(Rect rect) {
  Rect me = viewport(32);

  return Collision::isOverlapping(me, rect);
}

Point Camera::point() {
  return Point(x, y);
}

Rect Camera::viewport(float buffer) {
  return Rect{
    .x = x - buffer,
    .y = y - buffer,
    .w = logicalWidth + buffer*2,
    .h = logicalHeight + buffer*2
  };
}

float Camera::getWidth() {
  return logicalWidth / zoom;
}

float Camera::getHeight() {
  return logicalHeight / zoom;
}
