#include <battery.hpp>

#include <cmath>

#include <e/core.hpp>
#include <e/resources.hpp>

Battery::Battery() {
  sprite = new Spritesheet("battery.png", 18, 22);

  attachments = new Spritesheet("battery_attachments.png", 18, 22);
  attachments->hud = true;

  attachments->addAnimation("key", { 0 });
  attachments->addAnimation("chainsaw", { 2 });
  attachments->addAnimation("jumpers", { 4 });
  attachments->addAnimation("torch", { 6 });
  attachments->addAnimation("screen", { 8 });
  attachments->addAnimation("port", { 10 });

  width = sprite->width;
  height = sprite->height;
}

void Battery::start() {
  sprite->x = entity->scene->camera->getWidth() - (width + 1);
  sprite->y = entity->scene->camera->getHeight() - (height + 1);
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

  float x = sprite->x;
  float y = sprite->y;

  Rect dst1 = {
    x,
    y,
    width,
    height
  };

  SDL_Rect dst = scene->camera->toView(dst1, true);

  j.src = src;
  j.dst = dst;
  j.depth += DEPTH_ABOVE;
  scene->renderer->queue.push(j);

  sprite->frame = 1;
  src = sprite->getSRC();

  j.src = src;
  j.dst = dst;
  j.depth += DEPTH_ABOVE;
  scene->renderer->queue.push(j);

  float battSize = (int) (height * capacity);
  int offset = height - battSize;

  sprite->frame = 2;
  src = sprite->getSRC();

  src.y += offset;
  src.h = battSize;

  dst1 = {
    x,
    y + offset,
    width,
    (float) battSize
  };

  dst = scene->camera->toView(dst1, true);

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
