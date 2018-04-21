#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <e/resources.hpp>
#include <e/csv.hpp>

struct Tilemap {
  SDL_Texture* texture;

  int textureWidth;
  int textureHeight;

  int tileSize;

  std::vector<std::vector<std::string>> data;

  void load(std::string level, std::string tilesetName, int tileSize = 16);
  void render(SDL_Renderer* renderer, SDL_Point camera);
};
