#pragma once

#include <e/entity.hpp>
#include <e/sprite.hpp>

#include <SDL2/SDL.h>

struct Computer : Entity {
  void start();
  void tick(float dt);

  SDL_Color textColor = { 1, 255, 24, 255 };

  std::string input;

  Sprite* screenSprite;

  std::string buffer;
  SDL_Texture* bufferTexture;
  SDL_Rect bufferRect;
  void genBuffer();
};
