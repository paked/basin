#include <blockade.hpp>

#include <stdio.h>

// TODO: move this somewhere else
float rand01() {
  return (float) rand()/RAND_MAX;
}

Blockade::Blockade(float x, float y) {
  sprite = new Sprite("maze_blockade.png", x, y);
}

void Blockade::start() {
  float x = sprite->x;
  float y = sprite->y;

  explosionSprite = new Spritesheet("explosion.png", 32, 32, x - 4, y + 16);
  explosionSprite->frameDuration = 1000/40;
  explosionSprite->addAnimation("none", { 0 });
  explosionSprite->addAnimation("explode", { 1, 2, 3, 4, 5, 6, 7, 8, 9});
  explosionSprite->playAnimation("none", false);
  explosionSprite->localDepth = DEPTH_UI;

  particleSprite = new Spritesheet("blockade_particle.png", 5, 8);

  int particleCount = sprite->height/particleSprite->height;
  for (int i = 0; i < particleCount; i++) {
    float px = x + 12;
    float py = (y + i*particleSprite->height);
    float dx = -1 * rand01();
    float dy = (rand01()*2 - 1);
    float ddx = 1 + rand01();
    float ddy = 1 + rand01();

    particles.push_back({
        px,
        py,
        dx,
        dy,
        ddx,
        ddy,
        rand() % 5}
        );
  }

  reg(sprite);
  reg(explosionSprite);
}

void Blockade::explode() {
  exploding = true;
  explosionSprite->playAnimation("explode", false);
  explosionStartParticlesTime = SDL_GetTicks() + explosionStartParticlesDelay;
  up = false;
  sprite->solid = false;
  sprite->visible = false;
}

void Blockade::tick(float dt) {
  if (exploding && SDL_GetTicks() > explosionStartParticlesTime) {
    particlesAlive = true;
    particleDeathTime = SDL_GetTicks() + particleAliveTime;
    exploding = false;
    up = false;
  }

  if (particlesAlive) {
    if (SDL_GetTicks() > particleDeathTime) {
      particlesAlive = false;
    }

    for (auto& p : particles) {
      p.dx *= p.ddx;
      p.dx *= p.ddy;

      p.x += p.dx * particleSpeed * dt;
      p.y += p.dy * particleSpeed * dt;
    }
  }

  if (particlesAlive) {
    for (auto p : particles) {
      particleSprite->x = p.x;
      particleSprite->y = p.y;

      particleSprite->frame = p.f;

      particleSprite->job(scene, getDepth() + DEPTH_FG + 2*DEPTH_ABOVE);
    }
  }

  Entity::tick(dt);
}
