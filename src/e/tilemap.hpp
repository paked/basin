#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/resources.hpp>
#include <e/csv.hpp>

struct Tilemap {
  SDL_Texture* texture;

  int textureWidth;
  int textureHeight;

  int tileSize;

  std::vector<std::vector<std::string>> data;

  bool canCollide = false;
  std::vector<std::vector<std::string>> collisionData;

  void loadLevel(std::string level, std::string tilesetName, int tileSize = 16);
  void loadCollision(std::string collision);

  void render(SDL_Renderer* renderer, SDL_Point camera);

  static void collide(Sprite *sprite, Tilemap *map);
};
