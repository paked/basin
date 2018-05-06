#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/tileset.hpp>
#include <e/tilelayer.hpp>
#include <e/group.hpp>

struct Tilemap {
  Tilemap(Tileset* ts);

  int loadLayer(std::string fname, float depth);
  void loadCollisionLayer(std::string fname);
  void addToGroup(Group<Entity> *g);

  Tileset* tileset;
  std::vector<Tilelayer*> layers;

  bool canCollide = false;
  Tilelayer::Data collisionData;
};
