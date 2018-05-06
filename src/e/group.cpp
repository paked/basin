#include <e/group.hpp>

void Group::add(Entity* e) {
  e->scene = scene;

  members.push_back(e);
}

void Group::tick(float dt) {
  for (auto e : members) {
    e->tick(dt);
  }
}

void Group::render(SDL_Renderer* renderer, Camera cam) {
  for (auto e : members) {
    // e->render(renderer, cam);
  } 
}
