#include <e/rect.hpp>

SDL_Rect Rect::toSDL(Rect r) {
  return SDL_Rect {
    .x = (int) r.x,
    .y = (int) r.y,
    .w = (int) r.w,
    .h = (int) r.h
  };
}
