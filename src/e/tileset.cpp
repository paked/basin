#include <e/tileset.hpp>

Tileset::Tileset(std::string tname, int size) : tileSize(size) {
  sprite = new Sprite(tname);
  sprite->spritesheet(tileSize, tileSize);
}
