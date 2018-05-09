#include <torch.hpp>

#include <e/core.hpp>

#include <config.hpp>

Torch::Torch() {
  beam = new Sprite("torch_beam.png");
}

void Torch::beamIn(Direction d) {
  SDL_Rect src = beam->rect();
  SDL_Rect dst = beam->rect();

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

  SDL_Point center = { .x = beam->width/2, .y = 8 * Core::scale };

  SDL_RenderCopyEx(
      Core::renderer,
      beam->texture,
      &src,
      &dst,
      angle,
      &center,
      SDL_FLIP_NONE);
}
