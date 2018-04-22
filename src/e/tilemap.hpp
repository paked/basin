#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/sprite.hpp>
#include <e/resources.hpp>
#include <e/csv.hpp>
#include <e/camera.hpp>

struct Tilemap {
  SDL_Texture* texture;

  int textureWidth;
  int textureHeight;

  int tileSize;

  std::vector<std::vector<std::string>> backgroundData;
  std::vector<std::vector<std::string>> foregroundData;

  bool canCollide = false;
  std::vector<std::vector<std::string>> collisionData;

  void loadTileset(std::string tilesetName, int tileSize = 16);

  void loadBackground(std::string level);
  void loadForeground(std::string level);
  void loadCollision(std::string collision);

  void renderBackground(SDL_Renderer* renderer, Camera camera);
  void renderForeground(SDL_Renderer* renderer, Camera camera);

  void renderLayer(std::vector<std::vector<std::string>> layer, SDL_Renderer* renderer, Camera camera);

  static void collide(Sprite *sprite, Tilemap *map);
};
