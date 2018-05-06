#include <e/entity.hpp>

void Entity::start() {}
void Entity::tick(float dt) {}
void Entity::postTick() {}

float Entity::getDepth() {
  float depth = localDepth;

  if (parent != nullptr) {
    depth += parent->getDepth();
  }

  return depth; 
}
