#include <e/tilemap.hpp>

#include <e/core.hpp>
#include <e/csv.hpp>

void Tilemap::loadTileset(std::string tilesetName, int ts) {
  tileSize = ts;

  tileset = new Sprite(tilesetName);
  tileset->spritesheet(tileSize, tileSize);
}

void Tilemap::loadBackground(std::string lvl) {
  CSV csv(lvl);
  backgroundData = csv.getDataInt();
}

void Tilemap::loadForeground(std::string lvl) {
  CSV csv(lvl);
  foregroundData = csv.getDataInt();
}

void Tilemap::loadCollision(std::string collision) {
  CSV csv(collision);
  collisionData = csv.getDataInt();

  canCollide = true;
}

void Tilemap::renderBackground(SDL_Renderer* renderer, Camera camera) {
  renderLayer(backgroundData, renderer, camera);
}

void Tilemap::renderForeground(SDL_Renderer* renderer, Camera camera) {
  renderLayer(foregroundData, renderer, camera);
}

void Tilemap::renderLayer(Data& data, SDL_Renderer* renderer, Camera camera) {
  for (int y = 0; y < data.size(); y++) {
    auto row = data[y];

    for (int x = 0; x < row.size(); x++) {
      auto tile = row[x];

      if (tile < 0) {
        continue;
      }

      SDL_Rect dst = {
        .x = x * tileSize * Core::scale,
        .y = y * tileSize * Core::scale,
        .w = tileSize * Core::scale,
        .h = tileSize * Core::scale
      };

      if (!camera.withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }

      // Translate dst to screen coordinates
      dst.x -= camera.x;
      dst.y -= camera.y;

      SDL_Rect src = tileset->getFrame(tile);

      SDL_RenderCopy(renderer, tileset->texture, &src, &dst);
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
      auto tile = row[x];

      if (tile < 0) {
        continue;
      }
      
      SDL_Rect rect = {
        .x = x * map->tileSize * Core::scale,
        .y = y * map->tileSize * Core::scale,
        .w = map->tileSize * Core::scale,
        .h = map->tileSize * Core::scale
      };

      Sprite::collide(sprite, rect);
    }
  }
}
