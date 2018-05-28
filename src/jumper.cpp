#include <jumper.hpp>

#include <config.hpp>

Jumper::Jumper(bool positive, Point socket) : socket(socket) {
  sprite = new Spritesheet("switchboard_gui_jumpers.png", 8, 24);
  sprite->hud = true;

  sprite->addAnimation("positive", { 0 });
  sprite->addAnimation("negative", { 1 });

  sprite->playAnimation((positive ? "positive" : "negative"));

  generateColliders();
}

Rect Jumper::rect() {
  return Rect {
    sprite->x + 2,
    sprite->y,
    4,
    sprite->height,
  };
}

void Jumper::generateColliders() {
  Rect bananaRect = rect(); 

  Rect socketRect = {
    socket.x - bananaRect.w/2,
    socket.y - bananaRect.h/2,
    bananaRect.w,
    bananaRect.h/2
  };

  socketLeftPad = {
    socketRect.x,
    socketRect.y,
    socketRect.w/2,
    sprite->height/2
  };

  socketLeftPad.x -= (socketLeftPad.w + socketPadding);

  socketRightPad = {
    socketRect.x,
    socketRect.y,
    socketRect.w/2,
    sprite->height/2
  };

  socketRightPad.x += (socketRightPad.w*2 + socketPadding);

  socketTopPad = {
    socketRect.x,
    socketRect.y,
    socketRect.w,
    socketRect.h/4
  };

  socketTopPad.y -= (socketTopPad.h + socketPadding);

  socketInPad = socketRect;
}

void Jumper::render(SDL_Renderer* renderer, Camera* camera) {
  sprite->render(renderer, camera);

#ifdef DEBUG
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
#endif

  if (in()) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 50, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 50, 10, 255);
  }
/*
  SDL_Rect okLight = {
    .x = socket.x - 4,
    .y = socket.y - 64,
    .w = 8,
    .h = 8
  };

  SDL_RenderFillRect(renderer, &okLight);*/
}

bool Jumper::dirty() {
  Rect r = rect();

  return Collision::isOverlapping(r, socketLeftPad) ||
    Collision::isOverlapping(r, socketRightPad) ||
    Collision::isOverlapping(r, socketTopPad);
}

bool Jumper::in() {
  return Collision::isOverlapping(sprite->rect(), socketInPad) && !dirty();
}
