#pragma once

#include <vector>

#include <SDL2/SDL.h>

struct Input {
  static std::vector<Input*> inputs;

  static void handle(SDL_KeyboardEvent event);
  static void push();

  Input(SDL_Scancode k);

  bool down();
  bool up();
  bool justDown();
  bool justUp();

  SDL_Scancode key;

  bool lastFrameDown = false;
  bool thisFrameDown = false;
};
