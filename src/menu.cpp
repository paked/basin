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

  pressEnter = new Text("Press enter", 30, x, y + 60);
  pressEnter->alignment = Text::CENTER;
  pressEnter->color = Text::Black;
}

void Menu::start() {
  reg(mountain);
  reg(title);
  reg(pressEnter);

  enterTimer.go();
}

void Menu::tick(float dt) {
  Entity::tick(dt);

  if (exitTimer.done()) {
    active = false;
  }

  if (enterTimer.done()) {
    pressEnter->active = !pressEnter->active;

    enterTimer.go();
  }
}

void Menu::exit() {
  exitTimer.go(1.0 * SECONDS);
}
