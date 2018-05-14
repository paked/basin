#pragma once

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

#include <e/point.hpp>
#include <e/scene.hpp>
#include <e/component.hpp>

typedef std::vector<int> Animation;

struct Sprite : Component {
  Sprite(std::string texName, float x=0, float y=0);

  void tick(float dt);

  void job(Scene* scene, float depth=0);
  void render(SDL_Renderer* renderer, SDL_Point camera);

  SDL_Rect rect();

  virtual SDL_Rect getSRC();

  // internal method. probably doesn't have much outside use.
  void updateAnimation();

  int x;
  int y;
  int width;
  int height;
  // should the image be flipped horizontally?
  bool flip = false;
  // render in camera space or on screen space?
  int angle = 0;

  bool hud = false;
  bool solid = true;
  bool visible = true;
  float localDepth = 0;

  Point velocity;
  Point acceleration;
  Point drag = Point(0.99, 0.99);
  Point maxVelocity = Point(1000, 1000);
  Point nextPositionDelta;

  int textureWidth;
  int textureHeight;
  SDL_Texture *texture;
};
