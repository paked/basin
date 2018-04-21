#include <e/tilemap.hpp>

void Tilemap::load(std::string level, std::string tilesetName, int ts) {
  tileSize = ts;

  texture = Resources::get(tilesetName, &textureWidth, &textureHeight);

  CSV csv(level);
  data = csv.getData();
}

void Tilemap::render(SDL_Renderer* renderer, SDL_Point camera) {
  int tilesPerRow = textureWidth/tileSize;
  int tilesPerColumn = textureHeight/tileSize;

  for (int y = 0; y < data.size(); y++) {
    auto row = data[y];

    for (int x = 0; x < row.size(); x++) {
      auto tile = stoi(row[x]);

      if (tile < 0) {
        continue;
      }

      int tileY = tile/tilesPerRow;
      int tileX = tile - tileY;

      SDL_Rect src = {
        .x = tileX * tileSize,
        .y = tileY * tileSize,
        .w = 16,
        .h = 16
      };

      SDL_Rect dst = {
        .x = x * tileSize,
        .y = y * tileSize,
        .w = 16,
        .h = 16
      };

      /*if (!camera.withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }*/

      dst.x -= camera.x;
      dst.y -= camera.y;

      SDL_RenderCopy(renderer, texture, &src, &dst);
    }
  }
}

