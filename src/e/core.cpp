#include <e/core.hpp>

#include <config.hpp>

SDL_Window* Core::window = nullptr;
SDL_Renderer* Core::renderer = nullptr;

void Core::init() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
}
