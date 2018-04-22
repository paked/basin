#include <e/tilemap.hpp>

#include <e/csv.hpp>

void Tilemap::loadTileset(std::string tilesetName, int ts) {
  tileSize = ts;

  texture = Resources::get(tilesetName, &textureWidth, &textureHeight);
}

void Tilemap::loadBackground(std::string lvl) {
  CSV csv(lvl);
  backgroundData = csv.getData();
}

void Tilemap::loadForeground(std::string lvl) {
  CSV csv(lvl);
  foregroundData = csv.getData();
}

void Tilemap::loadCollision(std::string collision) {
  CSV csv(collision);
  collisionData = csv.getData();

  canCollide = true;
}

void Tilemap::renderBackground(SDL_Renderer* renderer, Camera camera) {
  renderLayer(backgroundData, renderer, camera);
}

void Tilemap::renderForeground(SDL_Renderer* renderer, Camera camera) {
  renderLayer(foregroundData, renderer, camera);
}

void Tilemap::renderLayer(std::vector<std::vector<std::string>> data, SDL_Renderer* renderer, Camera camera) {
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
      int tileX = tile % tilesPerRow;

      SDL_Rect dst = {
        .x = x * tileSize,
        .y = y * tileSize,
        .w = tileSize,
        .h = tileSize
      };

      if (!camera.withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }

      SDL_Rect src = {
        .x = tileX * tileSize,
        .y = tileY * tileSize,
        .w = tileSize,
        .h = tileSize
      };

      dst.x -= camera.x;
      dst.y -= camera.y;

      SDL_RenderCopy(renderer, texture, &src, &dst);
    }
  }
}

void Tilemap::collide(Sprite *sprite, Tilemap *map) {
  if (!map->canCollide) {
    printf("Tilemap does not have collision data\n");

    return;
  }

  auto data = map->collisionData;

  for (int y = 0; y < data.size(); y++) {
    auto row = data[y];

    for (int x = 0; x < row.size(); x++) {
      auto tile = stoi(row[x]);

      if (tile < 0) {
        continue;
      }
      
      SDL_Rect rect = {
        .x = x * map->tileSize,
        .y = y * map->tileSize,
        .w = map->tileSize,
        .h = map->tileSize
      };

      Sprite::collide(sprite, rect);
    }
  }
}
