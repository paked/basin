#include <computer.hpp>

#include <config.hpp>

void Computer::start() {
  localDepth = DEPTH_UI;

  screenSprite = new Sprite("computer_gui.png");
  screenSprite->hud = true;

  screenSprite->x = SCREEN_WIDTH/2 - screenSprite->width/2;
  screenSprite->y = SCREEN_HEIGHT/2 - screenSprite->height/2;
  screenSprite->hud = true;

  reg(screenSprite);
}
