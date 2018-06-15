#pragma once

#include <string>

#include <e/entity.hpp>
#include <e/spritesheet.hpp>
#include <e/text.hpp>
#include <e/timer.hpp>

struct Boss : Entity {
  Boss(float x, float y);

  void start();

  void tick(float dt);

  void wakeUp();

  Spritesheet* sprite;

  float textboxX = 0;
  float textboxY = 0;
  int textboxWidth = 10;
  int textboxHeight = 4;
  Spritesheet* textboxSprite;
  Text* textboxText;
  float textboxTextOffsetX = 16;
  float textboxTextOffsetY = 16;

  bool awake = false;

  Timer speakDelay = Timer();
  Timer letterTimer = Timer(25);
  int letterIndex = 0;
  std::string speech = "Hello mortal. I am Xenu, ruler of the cosmos. Descendent of Zeus, slayer of Titans. You have come to feed me, correct?\n This is the end of the game. You may leave.";

  Timer enterTimer = Timer(300);
};
