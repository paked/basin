#include <e/text.hpp>

#include <SDL_ttf.h>

#include <e/resources.hpp>
#include <e/core.hpp>

Text::Text(std::string text, int font, float x, float y) : text(text), x(x), y(y) {
  TTF_Font* f = Resources::get(font);
  SDL_Surface* surface = TTF_RenderText_Solid(f, text.c_str(), White);

  texture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  rect.x = rect.y = 0;
  TTF_SizeText(f, text.c_str(), &rect.w, &rect.h);

  SDL_FreeSurface(surface);
}

void Text::center() {
  x -= rect.w/2;
  y -= rect.h/2;
}

void Text::tick(float dt) {
  Component::tick(dt);

  float zoom = entity->scene->camera->zoom;

  float alignedX = x;
  float alignedY = y;

  switch (alignment) {
    case LEFT:
      alignedX = y;
      alignedY = y;

      break;
    case CENTER:
      alignedX = x - rect.w/2;
      alignedY = y - rect.h/2;

      break;
    default:
      printf("Could not render text, invalid alignment!");

      return;
  }

  Rect dst = {
    alignedX,
    alignedY,
    rect.w,
    rect.h
  };

  RenderJob j;
  j.depth = entity->getDepth() + DEPTH_UI *100;
  j.tex = texture;
  j.src = rect;
  j.dst = entity->scene->camera->toView(dst);

  entity->scene->renderer->queue.push(j);
}

Text::~Text() {
  SDL_DestroyTexture(texture);
}

SDL_Color Text::White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
