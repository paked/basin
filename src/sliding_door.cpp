#include <sliding_door.hpp>

SlidingDoor::SlidingDoor(int x, int y) {
  sprite = new Sprite("sliding_door_vert.png", x, y);
  sprite->spritesheet(16, 48);

  sprite->addAnimation("closed", { 0 });
  sprite->addAnimation("open", { 1, 2, 3, 4, 5, 6, 7 });

  sprite->playAnimation("closed", false);
}

void SlidingDoor::open() {
  isOpen = true;

  sprite->playAnimation("open", false);
}

void SlidingDoor::tick(float dt) {
  sprite->tick(dt);
}

void SlidingDoor::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);
}

SDL_Rect SlidingDoor::rect() {
  if (!isOpen) {
    return SDL_Rect{
      .x = sprite->x,
        .y = sprite->y,
        .w = sprite->width,
        .h = sprite->height
    };
  }

  return SDL_Rect{0, 0, 0, 0};
}
