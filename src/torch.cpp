#include <torch.hpp>

#include <e/core.hpp>

#include <config.hpp>

Torch::Torch() {
  beam = new Sprite("torch_beam.png");
}

void Torch::beamIn(Direction d) {
  SDL_Rect src = Rect::toSDL(beam->rect());

  Rect pos = beam->rect();
  pos.x = entity->scene->camera->getWidth()/2 - src.w/2;
  pos.y = entity->scene->camera->getHeight()/2 - src.h/8*1;

  SDL_Rect dst = entity->scene->camera->toView(pos, true);

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

  angle -= 180;

  // whatever
  SDL_Point center = { .x = beam->width*2, .y = 16 };

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

  SDL_Rect r = Rect::toSDL(rect(d));
  SDL_RenderFillRect(Core::renderer, &r);
#endif
}

Rect Torch::rect(Direction d) {
  float sw = entity->scene->camera->getWidth();
  float sh = entity->scene->camera->getHeight();

  Rect r = beam->rect();
  r.w *= 0.6;

  float t = 0;

  switch (d) {
    case UP:
      r.x = sw/2 - r.w/2;
      r.y = sw/2 - r.h/8*7;

      break;
    case DOWN:
      r.x = sw/2 - r.w/2;
      r.y = sw/2 - r.h/8;

      break;
    case RIGHT:
      t = r.w;

      r.w = r.h;
      r.h = t;

      r.x = sw/2 - r.w/8;
      r.y = sw/2 - r.h/2;

      break;
    case LEFT:
      t = r.w;

      r.w = r.h;
      r.h = t;

      r.x = sw/2 - r.w/8*7;
      r.y = sw/2 - r.h/2;

      break;
  }

  return r;
}
