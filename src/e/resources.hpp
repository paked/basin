#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

struct Resources {
  static std::map<std::string, SDL_Texture*> textures;

  static SDL_Texture* get(std::string fname);
  static bool load(std::string fname);

  static std::string getPath(std::string name);

  static void clean();
};
