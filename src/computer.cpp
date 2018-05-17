#include <computer.hpp>

#include <e/text.hpp>
#include <e/resources.hpp>
#include <e/core.hpp>

#include <config.hpp>

void Computer::start() {
  localDepth = DEPTH_UI;

  screenSprite = new Sprite("computer_gui.png");
  screenSprite->hud = true;

  screenSprite->x = scene->camera->getWidth()/2 - screenSprite->width/2;
  screenSprite->y = scene->camera->getHeight()/2 - screenSprite->height/2;
  screenSprite->hud = true;

  reg(screenSprite);

  buffer = "It's damp, the floor is cold, and it smells of dead animal. Or something like that. Welcome to the innards of Echo Mountain. Around you is a small hole in the ground, a key, a computer, and a hammer.\n\n[If you don't know how to get started, type \"help\"]";

  genBuffer();
}

void Computer::tick(float dt) {
  Entity::tick(dt);

  Rect pos = {
    .x = 32 + 6,
    .y = 32 + 5,
    .w = (float) bufferRect.w,
    .h = (float) bufferRect.h
  };

  RenderJob j;
  j.src = bufferRect;
  j.dst = scene->camera->toView(pos, true);
  j.tex = bufferTexture;
  j.depth = getDepth() + DEPTH_ABOVE;

  scene->renderer->queue.push(j);
}

void Computer::genBuffer() {
  SDL_DestroyTexture(bufferTexture);

  TTF_Font* font = Resources::get(14);

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, buffer.c_str(), textColor, 128);

  bufferTexture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  bufferRect.x = bufferRect.y = 0;
  SDL_QueryTexture(bufferTexture, NULL, NULL, &bufferRect.w, &bufferRect.h);

  SDL_FreeSurface(surface);
}
