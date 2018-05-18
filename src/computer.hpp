#pragma once

#include <vector>
#include <string>

#include <e/entity.hpp>
#include <e/sprite.hpp>
#include <e/input.hpp>

#include <SDL2/SDL.h>

struct Computer : Entity {
  void start();
  void tick(float dt);
  void activate();
  void deactivate();

  void genBuffer();
  void genInput();
  void eval(std::string cmd);
  void send(std::string cmd, std::string resp="");
  std::string getBuffer();

  SDL_Color textColor = { 1, 255, 24, 255 };

  Sprite* screenSprite;

  Input enter = Input(SDL_SCANCODE_RETURN, true);
  Input up = Input(SDL_SCANCODE_UP, true);
  Input down = Input(SDL_SCANCODE_DOWN, true);

  int maxBufferHeight;
  int bufferOffset = 0;
  int maxLines = 10;
  std::vector<std::string> lines;
  SDL_Texture* bufferTexture;
  SDL_Rect bufferRect;

  std::string input;
  std::string lastInput;
  SDL_Texture* inputTexture;
  SDL_Rect inputRect;

  SDL_Rect screenRect;
};
