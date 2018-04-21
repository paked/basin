#include <e/core.hpp>

#include <SDL_ttf.h>

#include <config.hpp>

SDL_Window* Core::window = nullptr;
SDL_Renderer* Core::renderer = nullptr;
SDL_Color Core::clear;

void Core::init() {
  SDL_Init(SDL_INIT_VIDEO);

  if(TTF_Init()==-1) {
    printf("Unable to initialise SDL2_TTF: %s\n", TTF_GetError());
  }

  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

  SDL_RenderSetScale(renderer, GAME_SCALE, GAME_SCALE);
  SDL_CaptureMouse(SDL_TRUE);
}
