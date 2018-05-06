#include <e/tilelayer.hpp>

#include <e/core.hpp>

Tilelayer::Tilelayer(Tileset* ts, Data d, float dp) : tileset(ts), data(d) {
  depth = dp;
}

void Tilelayer::tick(float dt) {
  int tileSize = tileset->tileSize;

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

      if (!scene->camera->withinViewport(dst)) {
        // don't need to render if the thing isn't on screen
        continue;
      }

      // Translate dst to screen coordinates
      dst.x -= scene->camera->x;
      dst.y -= scene->camera->y;

      SDL_Rect src = tileset->sprite->getFrame(tile);

      RenderJob j;
      j.depth = depth;
      j.tex = tileset->sprite->texture;
      j.src = src;
      j.dst = dst;

      scene->renderer->queue.push(j);
    }
  }
}
