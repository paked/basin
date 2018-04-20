#pragma once

#include <SDL2/SDL.h>

// Pretty much a global dump for SDL level things
struct Core {
  static SDL_Window* window;
  static SDL_Renderer* renderer;

  static void init();
};
