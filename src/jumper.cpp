#include <jumper.hpp>

Jumper::Jumper(bool positive, SDL_Point socket) : socket(socket) {
  sprite = new Sprite("switchboard_gui_jumpers.png");
  sprite->hud = true;

  sprite->spritesheet(8, 24);
  sprite->addAnimation("positive", { 0 });
  sprite->addAnimation("negative", { 1 });

  sprite->playAnimation((positive ? "positive" : "negative"));
}

bool Jumper::dirty() {
  SDL_Rect rect = sprite->rect();

  SDL_Rect leftPad = { socket.x - 6, socket.y - 12, 2, 12 };
  SDL_Rect rightPad = { socket.x + 4, socket.y - 12, 2, 12 };
  SDL_Rect topPad = { socket.x - 2, socket.y - 14, 4, 2 };

  return SDL_HasIntersection(&rect, &leftPad) ||
         SDL_HasIntersection(&rect, &rightPad) ||
         SDL_HasIntersection(&rect, &topPad);
}

bool Jumper::in() {
  SDL_Rect rect = sprite->rect();

  SDL_Rect topPad = { socket.x - 2, socket.y - 10, 4, 2 };

  return SDL_HasIntersection(&rect, &topPad) && !dirty();
}
