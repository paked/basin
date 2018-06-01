#include <boss.hpp>

Boss::Boss(float x, float y) {
  sprite = new Spritesheet("face.png", 24, 32, x, y);
  sprite->center();

  sprite->addAnimation("dead", { 0 });
  sprite->addAnimation("boot", { 1, 2, 3, 5, 5, 5 });
  sprite->addAnimation("idle", { 5 });
  sprite->addAnimation("talking", { 5, 6, 7, 7, 6, 6, 7, 6, 7, 7, 6, 5, 6, 6, 6, 7, 7 });

  sprite->playAnimation("dead", true);

  sprite->frameDuration = 1000/20;
}

void Boss::start() {
  reg(sprite);
}

void Boss::tick(float dt) {
  Entity::tick(dt);

  if (!sprite->playing) {
    sprite->playAnimation("talking", false);
  }
}

void Boss::wakeUp() {
  awake = true;
  sprite->playAnimation("boot", false);
}
