#include <e/resources.hpp>

#include <SDL2/SDL_image.h>

#include <e/core.hpp>

#include <config.hpp>

std::map<std::string, SDL_Texture*> Resources::textures;

bool Resources::load(std::string name) {
  if (textures.find(name) != textures.end()) {
    printf("Texture has already been loaded: %s\n", name.c_str());

    return false;
  }

  std::string fname = getPath(name);

  SDL_Surface *image = IMG_Load(fname.c_str());
  if (!image) {
    printf("Texture could not been loaded: %s\n", IMG_GetError());

    return false;
  }

  textures[name] = SDL_CreateTextureFromSurface(Core::renderer, image);

  SDL_FreeSurface(image);

  return true;
}

SDL_Texture* Resources::get(std::string name) {
  if (textures.find(name) == textures.end()) {
    printf("Texture has not been loaded\n");

    return nullptr;
  }

  return textures.find(name)->second;
}

std::string Resources::getPath(std::string name) {
  return GFX_PATH + name;
}

void Resources::clean() {
  for (auto pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
}
