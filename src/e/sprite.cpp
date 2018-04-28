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

void Sprite::spritesheet(int fw, int fh) {
  isSpritesheet = true;

  frameWidth = fw;
  frameHeight = fh;

  width = fw * Core::scale;
  height = fh * Core::scale;
}

SDL_Rect Sprite::getFrame() {
  int f = 0;
  if (currentAnimation.size() != 0) {
    f = currentAnimation[currentFrame];
  }

  return getFrame(f);
}

SDL_Rect Sprite::getFrame(int i) {
  int xIndex = 0;
  int yIndex = 0;
  int rowSize = frameWidth;

  if (isSpritesheet) {
    rowSize = textureWidth/frameWidth;

    yIndex = i / rowSize;
    xIndex = i % rowSize;

    return SDL_Rect {
      .x = xIndex * frameWidth,
      .y = yIndex * frameHeight,
      .w = frameWidth,
      .h = frameHeight
    };
  }

  return SDL_Rect {
    .x = 0,
    .y = 0,
    .w = textureWidth,
    .h = textureHeight
  };
}

void Sprite::addAnimation(std::string name, Animation anim) {
  animations[name] = anim;
}

void Sprite::playAnimation(std::string name, bool l) {
  loop = l;

  playing = true;
  currentAnimation = animations[name];
  currentAnimationName = name;
  currentFrame = 0;
  nextFrame = SDL_GetTicks() + frameLength;
}

SDL_Rect Sprite::rect() {
  return SDL_Rect{
      .x = (int)x,
      .y = (int)y,
      .w = width,
      .h = height
  };
}

void Sprite::updateAnimation() {
  if (SDL_GetTicks() < nextFrame) {
    return;
  }

  nextFrame = SDL_GetTicks() + frameLength;

  // not at end of animation
  if (currentFrame < currentAnimation.size() - 1) {
    currentFrame++;

    return;
  }

  if (loop) {
    currentFrame = 0;

    return;
  }

  playing = false;
}

void Sprite::tick(float dt) {
  if (playing) {
    updateAnimation();
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
}

void Sprite::render(SDL_Renderer *renderer, SDL_Point camera) {
  int f = 0;
  if (currentAnimation.size() != 0) {
    f = currentAnimation[currentFrame];
  }

  renderFrame(f, renderer, camera);
}

void Sprite::renderFrame(int frame, SDL_Renderer* renderer, SDL_Point camera) {
  SDL_Rect dst = rect();
  SDL_Rect src = getFrame(frame);

  if (!hud) {
    dst.x -= camera.x;
    dst.y -= camera.y;
  }

  if (!flip) {
    SDL_RenderCopy(renderer, texture, &src, &dst);
    return;
  }

  SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void Sprite::collide(Sprite* first, SDL_Rect second) {
  SDL_Rect b = second;
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

bool Sprite::isOverlapping(SDL_Rect first, SDL_Rect second) {
  return SDL_HasIntersection(&first, &second);
}
