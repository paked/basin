#include <floorboard.hpp>

Floorboard::Floorboard(int x, int y) {
  localDepth = DEPTH_BG + DEPTH_BELOW * 10;
  sprite = new Sprite("floorboard.png", x, y);
}

void Floorboard::tick(float dt) {
  sprite->tick(dt);

  // Send render commands
  sprite->job(scene, getDepth());
}
