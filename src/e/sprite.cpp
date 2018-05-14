#include <e/sprite.hpp>

#include <e/core.hpp>
#include <e/resources.hpp>

Sprite::Sprite(std::string texName, float x, float y) : x(x), y(y) {
  x *= Core::scale;
  y *= Core::scale;

  texture = Resources::get(texName, &textureWidth, &textureHeight);

  width = textureWidth * Core::scale;
  height = textureHeight * Core::scale;
}

SDL_Rect Sprite::getSRC() {
  return SDL_Rect {
    .x = 0,
    .y = 0,
    .w = textureWidth,
    .h = textureHeight
  };
}

SDL_Rect Sprite::rect() {
  return SDL_Rect{
      .x = x,
      .y = y,
      .w = width,
      .h = height
  };
}

void Sprite::tick(float dt) {
  if (entity == nullptr) {
    printf("WARNING: calling tick on an entityless component!\n");
  }

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

  // Send render commands
  if (visible) {
    job(entity->scene, entity->getDepth() + localDepth);
  }
}

void Sprite::job(Scene* scene, float depth) {
  SDL_Rect dst = rect();
  SDL_Rect src = getSRC();

  if (!hud) {
    dst.x -= scene->camera->x;
    dst.y -= scene->camera->y;
  }

  SDL_RendererFlip fl = SDL_FLIP_NONE;

  if (flip) {
    fl = SDL_FLIP_HORIZONTAL;
  }

  RenderJob j;
  j.depth = depth;
  j.tex = texture;
  j.src = src;
  j.dst = dst;
  j.angle = angle;
  j.flip = fl;

  scene->renderer->queue.push(j);
}

void Sprite::render(SDL_Renderer* renderer, SDL_Point camera) {
  SDL_Rect dst = rect();
  SDL_Rect src = getSRC();

  if (!hud) {
    dst.x -= camera.x;
    dst.y -= camera.y;
  }

  SDL_RendererFlip f = SDL_FLIP_NONE;

  if (flip) {
    f = SDL_FLIP_HORIZONTAL;
  }

  SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, NULL, f);
}
