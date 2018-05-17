#include <floorboard.hpp>

#include <e/core.hpp>

Floorboard::Floorboard(int x, int y, bool fake) : fake(fake) {
  localDepth = DEPTH_BG + DEPTH_BELOW * 10;

  sprite = new Sprite("floorboard.png", x, y);

  light = new Sprite("light_small.png");

  light->x = x;
  light->y = y;

  reg(sprite);
}

void Floorboard::tick(float dt) {
  Entity::tick(dt);

  if (!glowing) {
    return;
  }

  if (glowingStartTime + glowDuration < SDL_GetTicks()) {
    light->alpha = 255;
    glowing = false;

    return;
  }

  float p = 1 - (SDL_GetTicks() - glowingStartTime)/(float)glowDuration;

  light->alpha = 255 * p;
}

void Floorboard::trigger() {
  glowing = true;

  glowingStartTime = SDL_GetTicks();
}

void Floorboard::glow() {
  if (!fake || !glowing) {
    return;
  }

  light->render(Core::renderer, scene->camera);
}
