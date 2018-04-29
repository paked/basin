#include <jumper.hpp>

Jumper::Jumper(bool positive, SDL_Point socket) : socket(socket) {
  sprite = new Sprite("switchboard_gui_jumpers.png");
  sprite->hud = true;

  sprite->spritesheet(8, 24);
  sprite->addAnimation("positive", { 0 });
  sprite->addAnimation("negative", { 1 });

  sprite->playAnimation((positive ? "positive" : "negative"));

  generateColliders();
}

SDL_Rect Jumper::rect() {
  return SDL_Rect {
    .x = sprite->x + 8,
    .y = sprite->y,
    .w = 16,
    .h = sprite->height,
  };
}

void Jumper::generateColliders() {
  SDL_Rect bananaRect = rect(); 

  SDL_Rect socketRect = {
    .x = socket.x - bananaRect.w/2,
    .y = socket.y - bananaRect.h/2,
    .w = bananaRect.w,
    .h = bananaRect.h/2
  };

  socketLeftPad = {
    .x = socketRect.x,
    .y = socketRect.y,
    .w = socketRect.w/2,
    .h = sprite->height/2
  };

  socketLeftPad.x -= (socketLeftPad.w + socketPadding);

  socketRightPad = {
    .x = socketRect.x,
    .y = socketRect.y,
    .w = socketRect.w/2,
    .h = sprite->height/2
  };

  socketRightPad.x += (socketRightPad.w*2 + socketPadding);

  socketTopPad = {
    .x = socketRect.x,
    .y = socketRect.y,
    .w = socketRect.w,
    .h = socketRect.h/4
  };

  socketTopPad.y -= (socketTopPad.h + socketPadding);

  socketInPad = socketRect;
}

void Jumper::render(SDL_Renderer* renderer, SDL_Point cam) {
  /* if (in()) {
    int hiddenLength = (socket.y - sprite->y)/4;
    int offset = (sprite->height - hiddenLength)/4;

    printf("hl: %d, os: %d\n", hiddenLength, offset);

    SDL_Rect src = sprite->getFrame();

    src.y += (24 - hiddenLength);
    src.h = hiddenLength;

    SDL_Rect dst = {
      .x = sprite->x,
      .y = sprite->y + hiddenLength*4,
      .w = sprite->width,
      .h = hiddenLength*4
    };

    SDL_RenderCopy(renderer, sprite->texture, &src, &dst);
  } else {*/
    sprite->render(renderer, cam);
  //}

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect bananaRect = rect(); 
  SDL_RenderDrawRect(renderer, &bananaRect);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderDrawRect(renderer, &socketInPad);

  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &socketLeftPad);

  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderDrawRect(renderer, &socketRightPad);

  SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &socketTopPad);

  if (in()) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 50, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 50, 10, 255);
  }

  SDL_Rect okLight = {
    .x = socket.x - 4,
    .y = socket.y - 64,
    .w = 8,
    .h = 8
  };

  SDL_RenderFillRect(renderer, &okLight);
}

bool Jumper::dirty() {
  SDL_Rect r = rect();

  return SDL_HasIntersection(&r, &socketLeftPad) ||
    SDL_HasIntersection(&r, &socketRightPad) ||
    SDL_HasIntersection(&r, &socketTopPad);
}

bool Jumper::in() {
  SDL_Rect r = sprite->rect();

  return SDL_HasIntersection(&r, &socketInPad) && !dirty();
}
