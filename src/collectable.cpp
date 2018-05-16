#include <collectable.hpp>

Collectable::Collectable(int x, int y, Type t) : type(t) {
  sprite = new Spritesheet("collectables.png", 8, 8, x, y);
}

void Collectable::start() {
  sprite->addAnimation("chainsaw", { 0 });
  sprite->addAnimation("key", { 1 });
  sprite->addAnimation("jumpers", { 2 });
  sprite->addAnimation("torch", { 3 });
  sprite->addAnimation("screen", { 4 });

  sprite->playAnimation(key(type));

  localDepth += DEPTH_ABOVE;

  reg(sprite);
}

std::string Collectable::key(Type t) {
  if (t == CHAINSAW) {
    return "chainsaw";
  } else if (t == KEY) {
    return "key";
  } else if (t == JUMPERS) {
    return "jumpers";
  } else if (t == TORCH) {
    return "torch";
  } else if (t == SCREEN) {
    return "screen";
  }

  return "";
}
