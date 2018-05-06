#pragma once

#include <e/entity.hpp>

struct Group : Entity{
  std::vector<Entity*> members;

  void add(Entity *e);

  void tick(float dt);
  void render(SDL_Renderer* renderer, Camera cam);
};
