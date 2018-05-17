#pragma once

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

#include <e/point.hpp>
#include <e/rect.hpp>
#include <e/scene.hpp>
#include <e/component.hpp>

struct Sprite : Component {
  Sprite(std::string texName, float x=0, float y=0);

  void tick(float dt);

  void job(Scene* scene, float depth=0);
  void render(SDL_Renderer* renderer, Camera* camera);

  Rect rect();
  virtual SDL_Rect getSRC();

  float x;
  float y;
  float width;
  float height;

  // should the image be flipped horizontally?
  bool flip = false;
  // render in camera space or on screen space?
  int angle = 0;
  char alpha = 255;

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
