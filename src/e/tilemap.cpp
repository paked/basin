#include <e/tilemap.hpp>

void Tilemap::loadLevel(std::string level, std::string tilesetName, int ts) {
  tileSize = ts;

  texture = Resources::get(tilesetName, &textureWidth, &textureHeight);

  CSV csv(level);
  data = csv.getData();
}

void Tilemap::loadCollision(std::string collision) {
  CSV csv(collision);
  collisionData = csv.getData();

  canCollide = true;
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
      int tileX = tile % tilesPerRow;

      SDL_Rect src = {
        .x = tileX * tileSize,
        .y = tileY * tileSize,
        .w = tileSize,
        .h = tileSize
      };

      SDL_Rect dst = {
        .x = x * tileSize,
        .y = y * tileSize,
        .w = tileSize,
        .h = tileSize
      };

      /* TODO: enable this again
      if (!camera.withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }
      */

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
