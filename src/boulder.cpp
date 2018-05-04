#include <boulder.hpp>

Boulder::Boulder(int x, int y) {
  sprite = new Sprite("boulder.png", x, y);
}

void Boulder::roll() {
  sprite->acceleration.x = -80;
}

void Boulder::tick(float dt) {
  sprite->angle += spinSpeed * dt;

  sprite->tick(dt);
}
