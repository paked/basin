#include <sliding_door.hpp>

SlidingDoor::SlidingDoor(float x, float y) {
  sprite = new Spritesheet("sliding_door_vert.png", 16, 48, x, y);
}

void SlidingDoor::start() {
  sprite->addAnimation("closed", { 0 });
  sprite->addAnimation("open", { 1, 2, 3, 4, 5, 6, 7 });

  sprite->playAnimation("closed", false);

  reg(sprite);
}

void SlidingDoor::open() {
  if (isOpen) return;

  isOpen = true;

  sprite->solid = false;

  sprite->playAnimation("open", false);
}

SDL_Rect SlidingDoor::rect() {
  return SDL_Rect{
    .x = sprite->x,
      .y = sprite->y,
      .w = sprite->width,
      .h = sprite->height
  };
}
