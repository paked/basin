#include <torch.hpp>

#include <e/core.hpp>

#include <config.hpp>

Torch::Torch() {
  beam = new Sprite("torch_beam.png");

  bufferSRC = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

  buffer = SDL_CreateTexture(Core::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bufferSRC.w, bufferSRC.h);

  SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_MOD);
}

void Torch::pre() {
  SDL_SetRenderTarget(Core::renderer, buffer);

  SDL_SetRenderDrawColor(Core::renderer, 0, 0, 0, dark ? 255 : 0);
  SDL_RenderClear(Core::renderer);
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

void Torch::job(Scene *scene) {
  RenderJob j;
  j.depth = 170; 
  j.tex = buffer;
  j.src = bufferSRC;
  j.dst = bufferSRC;

  scene->renderer->queue.push(j);
}

void Torch::post() {
  SDL_SetRenderTarget(Core::renderer, NULL);
}
