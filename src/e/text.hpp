#pragma once

#include <string>

#include <SDL2/SDL.h>

struct Text {
  Text(std::string text);

  std::string text;
  SDL_Texture* texture;

  SDL_Rect rect;

  static SDL_Color White;
};
