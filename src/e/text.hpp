#pragma once

#include <string>

#include <SDL.h>

#include <e/component.hpp>

struct Text : Component {
  // TODO: currently fonts are literally just tied to a size. This should not be the case, it's dumb.
  Text(std::string text, int font = 30, float x = 0, float y = 0);
  ~Text();

  void center();
  void tick(float dt);

  std::string text;
  SDL_Texture* texture;

  float x;
  float y;
  bool hud;

  SDL_Rect rect;

  static SDL_Color White;
};
