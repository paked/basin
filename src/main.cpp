#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <e/core.hpp>
#include <e/resources.hpp>

#include <game.hpp>

Game *game;

const float frameTimeMs = 1000.0/60.0;

void hook();

int main() {
  printf("Initialising SDL...");
  Core::init();
  printf(" Done.\n");

  printf("Initialising game...");
  game = new Game();
  printf(" Done\n");

  printf("Loading game resources...");
  game->load();
  printf(" Done.\n");

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(hook, 60, 1);
#else
  while (!game->quit) {
    int frameStart = SDL_GetTicks();

    hook();

    int frameEnd = SDL_GetTicks();
    int frameDuration = frameEnd - frameStart;

    if (frameDuration < frameTimeMs) {
      int delayFor = frameTimeMs - frameDuration;

      SDL_Delay(frameTimeMs - frameDuration);
    }
  }
#endif

  Resources::clean();

  return 0;
}

void hook() {
  game->tick((frameTimeMs)/100);

  SDL_Renderer* renderer = Core::renderer;

  SDL_SetRenderDrawColor(renderer, Core::clear.r, Core::clear.g, Core::clear.b, 255);
  SDL_RenderClear(renderer);

  game->render(renderer);

  SDL_RenderPresent(renderer);
}
