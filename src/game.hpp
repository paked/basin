#pragma once

#include <SDL2/SDL.h>

struct Game {
  void load();

  void tick(float dt);
  void render(SDL_Renderer *renderer);

  bool quit = false;

private:
  float x = 0;
  float y = 0;
};
