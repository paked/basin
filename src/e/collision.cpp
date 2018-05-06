#include <e/collision.hpp>

#include <e/core.hpp>

void Collision::collide(Sprite *sprite, Tilemap *map) {
  if (!map->canCollide) {
    printf("Tilemap does not have collision data\n");

    return;
  }

  auto data = map->collisionData;
  int tileSize = map->tileset->tileSize;

  for (int y = 0; y < data.size(); y++) {
    auto row = data[y];

    for (int x = 0; x < row.size(); x++) {
      auto tile = row[x];

      if (tile < 0) {
        continue;
      }
      
      SDL_Rect rect = {
        .x = x * tileSize * Core::scale,
        .y = y * tileSize * Core::scale,
        .w = tileSize * Core::scale,
        .h = tileSize * Core::scale
      };

      Sprite::collide(sprite, rect);
    }
  }
}

/*
// checks whether the player is on a background tile
bool Collision::isOverlapping(Sprite *sprite, Tilemap *map) {
  int x = sprite->x/Core::scale/map->tileset->tileSize;
  int y = sprite->y/Core::scale/map->tileSize;

  if (x < 0 || y < 0) {
    return false;
  }

  int tile = map->backgroundData[y][x];

  return tile > 0;
}*/
