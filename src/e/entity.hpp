#pragma once

#include <SDL2/SDL.h>

#include <e/scene.hpp>

const float DEPTH_BG = -100;
const float DEPTH_MG = 0;
const float DEPTH_FG = 100;

struct Entity {
  float depth = DEPTH_MG;

  Scene* scene;

  virtual void tick(float dt);
  virtual void render(SDL_Renderer* renderer, Camera cam);
};
