#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include <e/component.hpp>
#include <e/camera.hpp>

struct Entity {
  std::vector<Component*> components;

  void add(Component* c);
  void remove(Component* c);

  void tick(float dt);
  virtual void render(SDL_Renderer* renderer, Camera camera);
};
