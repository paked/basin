#include <collectable.hpp>

Collectable::Collectable(int x, int y, Type t) : type(t) {
  sprite = new Sprite("collectables.png");
  sprite->spritesheet(8, 8);

  sprite->addAnimation("chainsaw", { 0 });
  sprite->addAnimation("key", { 1 });

  sprite->x = x;
  sprite->y = y;

  sprite->playAnimation(key(type));
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

std::string Collectable::key(Type t) {
  if (t == CHAINSAW) {
    return "chainsaw";
  } else if (t == KEY) {
    return "key";
  }

  return "";
}
