#include <game.hpp>

#include <stdio.h>

#include <e/resources.hpp>

void Game::load() {
  bool ok = Resources::load("character.png");

  if (!ok) {
    printf("Could not load assets\n");
  }
}

void Game::tick(float dt) {
  x += 10 * dt;
  y += 10 * dt;
}

void Game::render(SDL_Renderer* renderer) {
  SDL_Rect dst = {
    .x = (int)x,
    .y = (int)y,
    .w = 64,
    .h = 64
  };

  SDL_RenderCopy(renderer, Resources::get("character.png"), NULL, &dst);
}
