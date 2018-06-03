#include <menu.hpp>

#include <SDL.h>

Menu::Menu(float x, float y) {
  mountain = new Sprite("mountain.png", 0,  0);

  localDepth = DEPTH_UI;

  Point c = mountain->getCenter();
  mountain->x = x - c.x;
  mountain->y = y - c.x;

  title = new Text("Echo Mountain", 60, x, y - 60);
  title->alignment = Text::CENTER;
}

void Menu::tick(float dt) {
  Entity::tick(dt);

  if (exitTimer.done()) {
    active = false;
  }
}

void Menu::start() {
  reg(mountain);
  reg(title);
}

void Menu::exit() {
  exitTimer.go(1.0 * SECONDS);
}
