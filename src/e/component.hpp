#pragma once

#include <e/camera.hpp>

struct Entity;

struct Component {
  Entity* e;

  Component(Entity* e);

  virtual void tick(float dt);
  virtual void render(SDL_Renderer* renderer, Camera camera);
};
