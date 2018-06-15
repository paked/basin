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
int startGame();

int main(int argc, char *argv[]) {
  printf("Initialising SDL...");
  Core::init();
  printf(" Done.\n");

  if (startGame() < 0) {
    printf("Can't start game.\n");

    return 0;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(hook, 60, 1);
#else
  while (!game->quit) {
    int frameStart = SDL_GetTicks();

    if (game->done) {
      delete game;

      startGame();
    }

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
  int frameStart = SDL_GetTicks();

  game->tick((frameTimeMs)/100);

  int tickEnd = SDL_GetTicks();
  int tickDuration = tickEnd - frameStart;

  SDL_Renderer* renderer = Core::renderer;

  SDL_SetRenderDrawColor(renderer, Core::clear.r, Core::clear.g, Core::clear.b, 255);
  SDL_RenderClear(renderer);

  game->render(renderer);
  int renderEnd = SDL_GetTicks();

  int renderDuration = renderEnd - tickEnd;
  int frameDuration = SDL_GetTicks() - frameStart;

  SDL_RenderPresent(renderer);
  if (frameDuration > 16) {
    printf("WARNING: frame took too long to complete (%d vs %d) (t: %d, r: %d).\n", frameDuration, 16, tickDuration, renderDuration);
  }
}

int startGame() {
  printf("Initialising game...");
  game = new Game();
  printf(" Done\n");

  printf("Loading game resources...");
  if (game->load()) {
    printf(" Done.\n");
  } else {
    printf(" Failed.\n");

    return -1;
  }

  printf("Starting game...");
  game->start();
  printf(" Done.\n");

  return 0;
}
