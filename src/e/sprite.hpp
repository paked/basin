#pragma once

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

#include <e/point.hpp>

typedef std::vector<int> Animation;

struct Sprite {
  Sprite(std::string texName, float x=0, float y=0);
  Sprite(std::string texName, float x, float y, int width, int height);

  void tick(float dt);
  void render(SDL_Renderer* renderer, SDL_Point camera);

  SDL_Rect rect();

  void spritesheet(int frameWidth, int frameHeight);
  SDL_Rect getFrame(int i);
  SDL_Rect getFrame();

  void playAnimation(std::string name, bool loop = true);
  void addAnimation(std::string name, Animation anim);

  bool isSpritesheet = false;

  int frameLength = 1000/15;
  int spritesheetWidth;
  int spritesheetHeight;

  Animation currentAnimation;
  int currentFrame = 0;
  bool loop = false;
  bool playing = false;
  int nextFrame;

  std::map<std::string, Animation> animations;

  // internal method
  void updateAnimation();

  int x;
  int y;

  int width;
  int height;

  Point nextPositionDelta;
  Point velocity;
  Point acceleration;
  Point drag = Point(0.99, 0.99);
  Point maxVelocity = Point(10, 10);

  SDL_Texture *texture;

  static void collide(Sprite* first, SDL_Rect second);
  static bool isOverlapping(SDL_Rect first, SDL_Rect second);
};
