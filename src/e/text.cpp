#include <e/text.hpp>

#include <SDL_ttf.h>

#include <e/resources.hpp>
#include <e/core.hpp>

Text::Text(std::string text) : text(text) {
  SDL_Surface* surface = TTF_RenderText_Solid(Resources::font, text.c_str(), White);

  texture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  TTF_SizeText(Resources::font, text.c_str(), &rect.w, &rect.h);

  rect.x = -rect.w/2;
  rect.y = -rect.h * 1.3;

  SDL_FreeSurface(surface);
}

SDL_Color Text::White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
