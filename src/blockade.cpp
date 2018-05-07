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

  explosionSprite = new Sprite("explosion.png", x - 16, y + 64);
  explosionSprite->frameLength = 1000/40;
  explosionSprite->spritesheet(32, 32);
  explosionSprite->addAnimation("none", { 0 });
  explosionSprite->addAnimation("explode", { 1, 2, 3, 4, 5, 6, 7, 8, 9});
  explosionSprite->playAnimation("none", false);

  particleSprite = new Sprite("blockade_particle.png");
  particleSprite->spritesheet(5, 8);
  particleSprite->addAnimation("things", {0, 1, 2, 3, 4});
  particleSprite->playAnimation("things");

  int particleCount = sprite->height/particleSprite->height;
  for (int i = 0; i < particleCount; i++) {
    float px = x + 12 * 4;
    float py = (y + i*particleSprite->height);
    float dx = -1 * rand01();
    float dy = (rand01()*2 - 1);
    float ddx = 1 + rand01();
    float ddy = 1 + rand01();

    particles.push_back({
        .x = px,
        .y = py,
        .dx = dx,
        .dy = dy,
        .ddx = ddx,
        .ddy = ddy,
        .f = rand() % 5}
        );
  }
}

void Blockade::explode() {
  exploding = true;
  explosionSprite->playAnimation("explode", false);
  explosionStartParticlesTime = SDL_GetTicks() + explosionStartParticlesDelay;
  up = false;
  sprite->solid = false;
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

  // send render commands
  if (up) {
    sprite->job(scene, getDepth());
  }

  explosionSprite->job(scene, DEPTH_FG + DEPTH_ABOVE);

  if (particlesAlive) {
    for (auto p : particles) {
      particleSprite->x = p.x;
      particleSprite->y = p.y;

      particleSprite->currentFrame = p.f;

      particleSprite->job(scene, getDepth() + DEPTH_FG + 2*DEPTH_ABOVE);
    }
  }
}
