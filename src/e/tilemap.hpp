#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/resources.hpp>
#include <e/csv.hpp>
#include <e/camera.hpp>

struct Tilemap {
  typedef std::vector<std::vector<int>> Data;
  SDL_Texture* texture;

  int textureWidth;
  int textureHeight;

  int tileSize;

  Data backgroundData;
  Data foregroundData;

  bool canCollide = false;
  Data collisionData;

  void loadTileset(std::string tilesetName, int tileSize = 16);

  void loadBackground(std::string level);
  void loadForeground(std::string level);
  void loadCollision(std::string collision);

  void renderBackground(SDL_Renderer* renderer, Camera camera);
  void renderForeground(SDL_Renderer* renderer, Camera camera);

  void renderLayer(Data& data, SDL_Renderer* renderer, Camera camera);

  static void collide(Sprite *sprite, Tilemap *map);
};
