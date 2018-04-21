#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

struct Resources {
  static std::map<std::string, SDL_Texture*> textures;
  static TTF_Font* font;

  static SDL_Texture* get(std::string fname);
  static SDL_Texture* get(std::string fname, int* width, int* height);
  static bool load(std::string fname);
  static bool loadFont(std::string fname, int fontSize);

  static std::string getPath(std::string name);

  static void clean();
};
