#include <info.hpp>

#include <stdio.h>
#include <cmath>

#include <e/resources.hpp>
#include <e/core.hpp>

#define DEGREES_TO_RADIANS(degrees) ((degrees) * 3.14159265359 / 180.0)

Info::Info(float x, float y) {
  sprite = new Sprite("info.png", x, y);

  starting = Point(x, y);

  SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

  const char* text = "go that way!";

  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Resources::font, text, White);

  textTexture = SDL_CreateTextureFromSurface(Core::renderer, surfaceMessage);

  textRect = SDL_Rect{
    .x = x,
    .y = y
  };

  TTF_SizeText(Resources::font, text, &textRect.w, &textRect.h);

  textRect.x = x - textRect.w/2;
  textRect.y = y - textRect.h * 1.3;

  SDL_FreeSurface(surfaceMessage);
}

void Info::tick(float dt) {
  sprite->y = (int) starting.y + std::sin(DEGREES_TO_RADIANS(SDL_GetTicks()/1.3))*1;

  sprite->tick(dt);
}

void Info::render(SDL_Renderer *renderer, SDL_Point cam) {
  sprite->render(renderer, cam);

  if (!showText) {
    return;
  }

  SDL_Rect dst = textRect;
  dst.x -= cam.x;
  dst.y -= cam.y;

  SDL_RenderCopy(renderer, textTexture, NULL, &dst);
}
