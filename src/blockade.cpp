#include <blockade.hpp>

#include <stdio.h>

// TODO: move this somewhere else
float rand01() {
  return (float) rand()/RAND_MAX;
}

Blockade::Blockade(float x, float y) {
  sprite = new Sprite("maze_blockade.png", x, y);

  explosionSprite = new Sprite("explosion.png", x - 16, y + 64);
  explosionSprite->frameLength = 1000/40;
  explosionSprite->spritesheet(32, 32);
  explosionSprite->addAnimation("none", { 0 });
  explosionSprite->addAnimation("explode", { 1, 2, 3, 4, 5, 6, 7, 8, 9});
  explosionSprite->playAnimation("none", false);

  particleSprite = new Sprite("blockade_particle.png");

  int particleCount = sprite->height/particleSprite->height;
  for (int i = 0; i < particleCount; i++) {
    float px = x + 12 * 4;
    float py = (y + i*particleSprite->height);
    float dx = -1 * rand01();
    float dy = (rand01()*2 - 1);

    particles.push_back({ px, py, dx, dy});
  }
}

void Blockade::explode() {
  exploding = true;
  explosionSprite->playAnimation("explode", false);
  explosionStartParticlesTime = SDL_GetTicks() + explosionStartParticlesDelay;
  up = false;
}

void Blockade::tick(float dt) {
  sprite->tick(dt);
  explosionSprite->tick(dt);

  if (exploding && SDL_GetTicks() > explosionStartParticlesTime) {
    particlesAlive = true;
    particleDeathTime = SDL_GetTicks() + particleAliveTime;
    exploding = false;
    up = false;
  }

  if (!particlesAlive) {
    return;
  }

  if (SDL_GetTicks() > particleDeathTime) {
    particlesAlive = false;

    return;
  }

  for (auto& p : particles) {
    p.x += p.dx * particleSpeed * dt;
    p.y += p.dy * particleSpeed * dt;
  }
}

void Blockade::render(SDL_Renderer* renderer, Camera cam) {
  if (up) {
    sprite->render(renderer, cam.point());
  }

  if (particlesAlive) {
    for (auto p : particles) {
      particleSprite->x = p.x;
      particleSprite->y = p.y;

      particleSprite->render(renderer, cam.point());
    }
  }
  
  explosionSprite->render(renderer, cam.point());
}