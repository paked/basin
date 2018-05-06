#pragma once

#include <e/tilemap.hpp>

struct Collision {
  static void collide(Sprite* sprite, Tilemap* tilemap);

  // static bool isOverlapping(Sprite* sprite, Tilemap* tilemap);
};
