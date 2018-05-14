#include <e/collision.hpp>

#include <e/core.hpp>

void Collision::collide(Sprite* first, Sprite* second) {
  if (!second->solid) {
    return;
  }

  collide(first, second->rect());
}

void Collision::collide(Sprite* first, SDL_Rect second) {
  if (!first->solid) {
    return;
  }

  SDL_Rect b = second;
  SDL_Rect res;

  // First case we check against next y position
  {
    SDL_Rect a = first->rect();
    a.y += (int)first->nextPositionDelta.y;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.y < 0) ? 1 : -1;
      first->nextPositionDelta.y += (int)res.h * mod;
    }
  }

  // Then we check against the next x position
  {
    SDL_Rect a = first->rect();
    a.x += (int)first->nextPositionDelta.x;

    if (SDL_IntersectRect(&a, &b, &res)) {
      float mod = (first->velocity.x < 0) ? 1 : -1;
      first->nextPositionDelta.x += (int)res.w * mod;
    }
  }
}

void Collision::collide(Sprite *sprite, Tilemap *map) {
  if (!map->canCollide) {
    printf("Tilemap does not have collision data\n");

    return;
  }

  auto data = map->collisionData;
  int tileSize = map->tileset->frameWidth;

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

      Collision::collide(sprite, rect);
    }
  }
}

bool Collision::isOverlapping(Sprite* first, SDL_Rect second) {
  SDL_Rect frect = first->rect();

  return isOverlapping(frect, second);
}

bool Collision::isOverlapping(SDL_Rect first, SDL_Rect second) {
  return SDL_HasIntersection(&first, &second);
}

bool Collision::isOverlapping(Sprite* sprite, Tilemap *map, int layer) {
  int tileSize = map->tileset->frameWidth;

  int x = sprite->x/Core::scale/tileSize;
  int y = sprite->y/Core::scale/tileSize;

  Tilelayer* l = map->layers[layer];

  return l->data[y][x] > 0;
}
