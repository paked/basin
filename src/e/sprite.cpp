#include <e/sprite.hpp>

#include <e/resources.hpp>

Sprite::Sprite(std::string texName, float x, float y) : x(x), y(y) {
  texture = Resources::get(texName, &width, &height);
}

Sprite::Sprite(std::string texName, float x, float y, int width, int height) : x(x), y(y), width(width), height(height) {
  texture = Resources::get(texName);
}

SDL_Rect Sprite::rect() {
  return SDL_Rect{
      .x = (int)x,
      .y = (int)y,
      .w = width,
      .h = height
  };
}

void Sprite::tick(float dt) {
  x += (int)nextPositionDelta.x;
  y += (int)nextPositionDelta.y;

  velocity.x += (acceleration.x - drag.x * velocity.x) * dt;
  if (velocity.x > maxVelocity.x) {
    velocity.x = maxVelocity.x;
  } else if (velocity.x < -maxVelocity.x) {
    velocity.x = -maxVelocity.x;
  }

  velocity.y += (acceleration.y - drag.y * velocity.y) * dt;
  if (velocity.y > maxVelocity.y) {
    velocity.y = maxVelocity.y;
  } else if (velocity.y < -maxVelocity.y) {
    velocity.y = -maxVelocity.y;
  }

  nextPositionDelta.x = velocity.x * dt;
  nextPositionDelta.y = velocity.y * dt;
}

void Sprite::render(SDL_Renderer *renderer, SDL_Point camera) {
  SDL_Rect dst = rect();

  dst.x -= camera.x;
  dst.y -= camera.y;

  SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void Sprite::collide(Sprite* first, Sprite* second) {
  SDL_Rect b = second->rect();
  SDL_Rect res;

  // First case we check against next y position
  {
    SDL_Rect a = first->rect();
    a.y += (int)first->nextPositionDelta.y;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.y < 0) ? 1 : -1;
      first->nextPositionDelta.y += (int)res.h * mod;
    }
  }

  // Then we check against the next x position
  {
    SDL_Rect a = first->rect();
    a.x += (int)first->nextPositionDelta.x;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.x < 0) ? 1 : -1;
      first->nextPositionDelta.x += (int)res.w * mod;
    }
  }
}
