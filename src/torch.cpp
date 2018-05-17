#include <torch.hpp>

#include <e/core.hpp>

#include <config.hpp>

Torch::Torch() {
  beam = new Sprite("torch_beam.png");
}

void Torch::beamIn(Direction d) {
  SDL_Rect src = Rect::toSDL(beam->rect());
  SDL_Rect dst = entity->scene->camera->toView(beam->rect(), false);

  dst.x = SCREEN_WIDTH/2 - src.w/2;
  dst.y = SCREEN_WIDTH/2 - src.h/8;

  double angle = 0;

  switch (d) {
    case UP:
      angle = 0;

      break;
    case DOWN:
      angle = 180;

      break;
    case LEFT:
      angle = 270;

      break;
    case RIGHT:
      angle = 90;

      break;
    default:
      printf("Can't beam torch... Unknown direction?\n");
  }

  // for some reason...
  angle -= 180;

  SDL_Point center = { .x = beam->width/2, .y = 8 };

  SDL_RenderCopyEx(
      Core::renderer,
      beam->texture,
      NULL,
      &dst,
      angle,
      &center,
      SDL_FLIP_NONE);

#ifdef DEBUG
  SDL_SetRenderDrawColor(Core::renderer, 255, 255, 255, 255);

  SDL_Rect r = rect(d);
  SDL_RenderFillRect(Core::renderer, &r);
#endif
}

Rect Torch::rect(Direction d) {
  Rect r = beam->rect();
  r.w *= 0.6;

  float t = 0;

  switch (d) {
    case UP:
      r.x = SCREEN_WIDTH/2 - r.w/2;
      r.y = SCREEN_WIDTH/2 - r.h/8*7;

      break;
    case DOWN:
      r.x = SCREEN_WIDTH/2 - r.w/2;
      r.y = SCREEN_WIDTH/2 - r.h/8;

      break;
    case RIGHT:
      t = r.w;

      r.w = r.h;
      r.h = t;

      r.x = SCREEN_WIDTH/2 - r.w/8;
      r.y = SCREEN_WIDTH/2 - r.h/2;

      break;
    case LEFT:
      t = r.w;

      r.w = r.h;
      r.h = t;

      r.x = SCREEN_WIDTH/2 - r.w/8*7;
      r.y = SCREEN_WIDTH/2 - r.h/2;

      break;
  }

  return r;
}
