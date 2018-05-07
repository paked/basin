#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include <e/camera.hpp>
#include <e/sprite.hpp>
#include <e/entity.hpp>

struct Particle {
  float x;
  float y;

  float dx;
  float dy;

  float ddx;
  float ddy;

  int f;
};

struct Blockade : Entity {
  Blockade(float x, float y);

  void start();
  void tick(float dt);

  void explode();

  bool up = true;

  Sprite* sprite;

  bool exploding = false;
  int explosionStartParticlesDelay = 150;
  int explosionStartParticlesTime;
  Sprite* explosionSprite;

  bool particlesAlive = false;
  float particleSpeed = 200;
  int particleAliveTime = 400;
  int particleDeathTime;
  Sprite* particleSprite;
  std::vector<Particle> particles;
};
