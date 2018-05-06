#pragma once

#include <SDL2/SDL.h>

#include <e/scene.hpp>

const float DEPTH_BG = -100;
const float DEPTH_MG = 0;
const float DEPTH_FG = 100;
const float DEPTH_ABOVE = 1;
const float DEPTH_BELOW = -1;

struct Entity {
  Entity* parent = nullptr;

  bool active = true;
  float localDepth = DEPTH_MG;

  Scene* scene;

  float getDepth();

  virtual void start();
  virtual void tick(float dt);
  virtual void postTick();
};
