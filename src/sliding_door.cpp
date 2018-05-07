#include <sliding_door.hpp>

SlidingDoor::SlidingDoor(int x, int y) {
  sprite = new Sprite("sliding_door_vert.png", x, y);
  sprite->spritesheet(16, 48);
}

void SlidingDoor::start() {
  sprite->addAnimation("closed", { 0 });
  sprite->addAnimation("open", { 1, 2, 3, 4, 5, 6, 7 });

  sprite->playAnimation("closed", false);
}

void SlidingDoor::open() {
  if (isOpen) return;

  isOpen = true;

  sprite->solid = false;

  sprite->playAnimation("open", false);
}

void SlidingDoor::tick(float dt) {
  sprite->tick(dt);

  sprite->job(scene);
}

SDL_Rect SlidingDoor::rect() {
  return SDL_Rect{
    .x = sprite->x,
      .y = sprite->y,
      .w = sprite->width,
      .h = sprite->height
  };
}
