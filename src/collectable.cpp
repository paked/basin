#include <collectable.hpp>

Collectable::Collectable(int x, int y) {
  sprite = new Sprite("collectables.png");
  sprite->spritesheet(8, 8);

  sprite->addAnimation("chainsaw", { 0 });
  sprite->playAnimation("chainsaw", false);

  sprite->x = x;
  sprite->y = y;
}

void Collectable::tick(float dt) {
  sprite->tick(dt);
}

void Collectable::render(SDL_Renderer* renderer, SDL_Point cam) {
  if (!active) {
    return;
  }

  sprite->render(renderer, cam);
}
