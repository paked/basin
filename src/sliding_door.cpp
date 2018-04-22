#include <sliding_door.hpp>

SlidingDoor::SlidingDoor(int x, int y) {
  sprite = new Sprite("sliding_door.png", x, y);
  sprite->spritesheet(32, 16);

  sprite->addAnimation("closed", { 0 });
  sprite->addAnimation("open", { 1, 2, 3, 4, 5 });

  sprite->playAnimation("open", false);
}

void SlidingDoor::open() {
  isOpen = true;

  sprite->playAnimation("open");
}

void SlidingDoor::tick(float dt) {
  sprite->tick(dt);
}

void SlidingDoor::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}

SDL_Rect SlidingDoor::rect() {
  return SDL_Rect{
    .x = sprite->x,
    .y = sprite->y + sprite->height,
    .w = sprite->width,
    .h = sprite->height
  };
}
