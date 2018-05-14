#include <floorboard.hpp>

#include <e/core.hpp>

Floorboard::Floorboard(int x, int y) {
  localDepth = DEPTH_BG + DEPTH_BELOW * 10;

  sprite = new Sprite("floorboard.png", x, y);

  light = new Sprite("light_small.png", x + 8 * Core::scale, y + 8 * Core::scale);

  reg(sprite);
}

void Floorboard::tick(float dt) {
  Entity::tick(dt);
}

void Floorboard::glow() {
}
