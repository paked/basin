#pragma once

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

#include <e/point.hpp>
#include <e/scene.hpp>

typedef std::vector<int> Animation;

struct Sprite {
  static void collide(Sprite* first, SDL_Rect second);
  static bool isOverlapping(SDL_Rect first, SDL_Rect second);

  Sprite(std::string texName, float x=0, float y=0);

  void tick(float dt);

  void job(Scene* scene, float depth=0);
  void render(SDL_Renderer* renderer, SDL_Point camera);
  void renderFrame(int frame, SDL_Renderer* renderer, SDL_Point camera);

  SDL_Rect rect();

  void spritesheet(int frameWidth, int frameHeight);
  SDL_Rect getFrame(int i);
  SDL_Rect getFrame();

  void playAnimation(std::string name, bool loop = true);
  void addAnimation(std::string name, Animation anim);

  // internal method. probably doesn't have much outside use.
  void updateAnimation();

  int x;
  int y;
  int width;
  int height;
  // should the image be flipped horizontally?
  bool flip = false;
  // render in camera space or on screen space?
  bool hud = false;
  int angle = 0;

  Point velocity;
  Point acceleration;
  Point drag = Point(0.99, 0.99);
  Point maxVelocity = Point(1000, 1000);
  Point nextPositionDelta;

  bool isSpritesheet = false;

  int frameLength = 1000/3;
  int frameWidth;
  int frameHeight;

  Animation currentAnimation;
  std::string currentAnimationName;
  std::map<std::string, Animation> animations;
  int currentFrame = 0;
  bool loop = false;
  bool playing = false;
  int nextFrame;

  int textureWidth;
  int textureHeight;
  SDL_Texture *texture;
};
