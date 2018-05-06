#pragma once

#include <e/entity.hpp>

template<class T = Entity>
struct Group : Entity {
  std::vector<T*> members;

  void add(T* e) {
    members.push_back(e);

    e->scene = scene;

    e->start();
  };

  void tick(float dt) {
    for (auto& m : members) {
      if (!m->active) {
        continue;
      }

      m->tick(dt);
    }
  }

  void postTick() {
    for (auto& m : members) {
      m->postTick();
    }
  }
};
