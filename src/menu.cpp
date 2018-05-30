#include <menu.hpp>

#include <SDL.h>

void Menu::start() {
  mountain = new Sprite("mountain.png", 0,  0);
  title = new Text("3 Years of Echo Mountain!", 60, scene->camera->getWidth()/2, 0); // scene->camera->getWidth()/2, scene->camera->getHeight()/2);
  title->center();

  reg(mountain);
  // reg(title);
}

void Menu::tick(float dt) {
  Entity::tick(dt);

  if (exiting) {
    int end= exitingStartTime + exitingDuration;
    int now = SDL_GetTicks();

    if (end < now) {
      done = true;
      active = false;

      printf("done!\n");
    }

    float p = 1.0 - ((float) (end - now)) / exitingDuration;

    printf("p: %f\n", p);

    mountain->y = yOffsetMountainStart + yOffsetTotal * (p*p);

    printf("y: %f\n", mountain->y);
  }

  // Draw text
  SDL_Rect rect = title->rect;

  SDL_Rect dst = {
    400 - rect.w/2,
    100 - rect.h/2,
    rect.w,
    rect.h
  };

  RenderJob j;
  j.depth = DEPTH_UI;
  j.tex = title->texture;
  j.src = title->rect;
  j.dst = dst;

  scene->renderer->queue.push(j);
}

void Menu::exit() {
  exiting = true;
  exitingStartTime = SDL_GetTicks();

  yOffsetMountainStart = mountain->y;
}
