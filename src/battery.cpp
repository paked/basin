#include <battery.hpp>

#include <cmath>

#include <e/core.hpp>
#include <e/resources.hpp>

#define DEGREES_TO_RADIANS(degrees) ((degrees) * 3.14159265359 / 180.0)

Battery::Battery(int x, int y) {
  sprite = new Spritesheet("battery.png", 18, 22);

  attachments = new Spritesheet("battery_attachments.png", 18, 22);
  attachments->hud = true;

  attachments->addAnimation("key", { 0 });
  attachments->addAnimation("chainsaw", { 2 });
  attachments->addAnimation("jumpers", { 4 });
  attachments->addAnimation("torch", { 6 });

  width = sprite->width;
  height = sprite->height;
}

void Battery::start() {
  sprite->x = entity->scene->camera->width - (width + 4);
  sprite->y = entity->scene->camera->height- (height + 4);
}

void Battery::attach(Collectable::Type type) {
  showAttachment = true;

  attachments->playAnimation(Collectable::key(type));
}

void Battery::unattach() {
  showAttachment = false;
}

void Battery::tick(float dt) {
  attachments->x = sprite->x;
  attachments->y = sprite->y - sprite->height + 2;

  // send render commands
  Scene* scene = entity->scene;
  RenderJob j;
  j.depth = DEPTH_UI;
  j.tex = sprite->texture;

  sprite->frame = 0;
  SDL_Rect src = sprite->getSRC();

  int x = sprite->x;
  int y = sprite->y;

  SDL_Rect dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  j.src = src;
  j.dst = dst;
  j.depth += DEPTH_ABOVE;
  scene->renderer->queue.push(j);

  sprite->frame = 1;
  src = sprite->getSRC();

  dst = {
    .x = x,
    .y = y,
    .w = width,
    .h = height
  };

  j.src = src;
  j.dst = dst;
  j.depth += DEPTH_ABOVE;
  scene->renderer->queue.push(j);

  int battSize = (int) (height * capacity);
  int offset = height - battSize;

  sprite->frame = 2;
  src = sprite->getSRC();

  src.y += offset/Core::scale;
  src.h = battSize;

  dst = {
    .x = x,
    .y = y + offset,
    .w = width,
    .h = battSize
  };

  j.src = src;
  j.dst = dst;
  j.depth += DEPTH_ABOVE;
  scene->renderer->queue.push(j);

  if (showAttachment) {
    attachments->job(scene, DEPTH_UI);
  }
}

bool Battery::hasCapacity(float c) {
  return (capacity - c > 0);
}
