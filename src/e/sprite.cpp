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
  x += velocity.x * dt;
  y += velocity.y * dt;

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
}

void Sprite::render(SDL_Renderer *renderer) {
  SDL_Rect dst = rect();

  SDL_RenderCopy(renderer, texture, NULL, &dst);
}
