#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/tileset.hpp>

struct Tilelayer : Entity {
  typedef std::vector<std::vector<int>> Data;

  Tilelayer(Tileset* ts, Data d, float dp);
  
  Tileset* tileset;
  Data data;

  void tick(float dt);
};
