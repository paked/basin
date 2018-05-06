#pragma once

#include <e/sprite.hpp>

struct Tileset {
  Tileset(std::string s, int ts);

  Sprite* sprite;
  int tileSize;
};
