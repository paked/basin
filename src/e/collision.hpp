#pragma once

#include <e/tilemap.hpp>

struct Collision {
  static void collide(Sprite* first, SDL_Rect second);
  static void collide(Sprite* sprite, Tilemap* tilemap);

  static bool isOverlapping(Sprite* first, SDL_Rect second);
  static bool isOverlapping(Sprite* sprite, Tilemap* tilemap, int layer);
};
