#include <computer_room_wall.hpp>

// Modified from https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c#L272
float bounceEaseOut(float p)
{
  if(p < 4/11.0)
  {
    return (121 * p * p)/16.0;
  }
  else if(p < 8/11.0)
  {
    return (363/40.0 * p * p) - (99/10.0 * p) + 17/5.0;
  }
  else if(p < 9/10.0)
  {
    return (4356/361.0 * p * p) - (35442/1805.0 * p) + 16061/1805.0;
  }
  else
  {
    return (54/5.0 * p * p) - (513/25.0 * p) + 268/25.0;
  }
}

ComputerRoomWall::ComputerRoomWall(float x, float y) {
  startX = x - distX;

  sprite = new Sprite("computer_room_wall.png", startX, y);
  sprite->solid = false;

  trigger = Rect{
    .x = x + sprite->width*2,
    .y = y,
    .w = sprite->width,
    .h = sprite->height
  };
}

void ComputerRoomWall::start() {
  localDepth = DEPTH_FG + DEPTH_ABOVE*2;
  reg(sprite);
}

void ComputerRoomWall::tick(float dt) {
  Entity::tick(dt);

  if (!bouncing) {
    return;
  }

  int bounceEndTime = bounceStartTime + enterDuration;
  int now = SDL_GetTicks();

  if (now > bounceEndTime) {
    sprite->x = startX + distX;
    sprite->solid = true;

    bouncing = false;

    return;
  }

  float p = 1.0 - ((float) (bounceEndTime - now)) / enterDuration;
  float bouncedP = bounceEaseOut(p);

  if (bouncedP > 0.65) {
    scene->camera->shake(1 * 100, 0.25);
  }

  sprite->x = startX + distX * bouncedP;
}

void ComputerRoomWall::enter() {
  bouncing = true;

  bounceStartTime = SDL_GetTicks();
}
