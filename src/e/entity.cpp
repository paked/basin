#include <e/entity.hpp>

#include <stdio.h>

void Entity::add(Component* c) {
  components.push_back(c);
}

void Entity::remove(Component* c) {
  // TODO: implement

  printf("can't remove: not implemented\n");
}

void Entity::tick(float dt) {
  for (auto c : components) {
    c->tick(dt);
  }
}

void Entity::render(SDL_Renderer* renderer, Camera camera) {
  for (auto c : components) {
    c->render(renderer, camera);
  }
}
