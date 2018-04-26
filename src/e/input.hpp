#pragma once

#include <vector>

#include <SDL2/SDL.h>

struct Input {
  static std::vector<Input*> inputs;

  static void handle(SDL_MouseButtonEvent event);
  static void handle(SDL_MouseMotionEvent event);
  static void handle(SDL_KeyboardEvent event);
  static void push();

  static int mouseX;
  static int mouseY;

  static bool mouseJustDown();
  static bool mouseJustUp();
  static bool mouseDown();
  static bool mouseUp();

  static bool mouseLastFrameDown;
  static bool mouseThisFrameDown;

  Input(SDL_Scancode k);

  bool down();
  bool up();
  bool justDown();
  bool justUp();

  SDL_Scancode key;

  bool lastFrameDown = false;
  bool thisFrameDown = false;
};
