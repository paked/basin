#include <boss.hpp>

#include <ctype.h>

Boss::Boss(float x, float y) {
  sprite = new Spritesheet("face.png", 24, 32, x, y);
}

void Boss::start() {
  sprite->center();

  sprite->addAnimation("dead", { 0 });
  sprite->addAnimation("boot", { 1, 2, 3, 5, 5, 5 });
  sprite->addAnimation("idle", { 5 });
  sprite->addAnimation("talking", { 5, 6, 7, 7, 6, 6, 7, 6, 7, 7, 6, 5, 6, 6, 6, 7, 7 });

  sprite->playAnimation("dead", true);

  sprite->frameDuration = 1000/20;

  textboxSprite = new Spritesheet("textbox.png", 16, 16);

  textboxX = scene->camera->getWidth()/2 - textboxWidth*16/2;
  textboxY = scene->camera->getHeight() - textboxHeight*16 + 4;

  textboxText = new Text("", 30, textboxX + textboxTextOffsetX, textboxY + textboxTextOffsetY, (textboxWidth - 2)*16);
  textboxText->hud = true;
  textboxText->localDepth = DEPTH_UI + DEPTH_ABOVE;
  textboxText->active = false;

  reg(sprite);
  reg(textboxText);

}

void Boss::tick(float dt) {
  Entity::tick(dt);

  if (speakDelay.done()) {
    letterTimer.go();
  }

  if (letterTimer.done() && letterIndex < speech.size()) {
    letterIndex += 1;

    textboxText->text = speech.substr(0, letterIndex);

    if (speech[letterIndex] == ' ') {
      speakDelay.go(75);
    } else if (isupper(speech[letterIndex])) {
      scene->camera->shake(20, 0.2);

      letterTimer.go();
    } else {
      letterTimer.go();
    }
  }

  if (letterIndex >= speech.size()) {
    sprite->playAnimation("idle");
    done = true;
  }

  if (!sprite->playing && sprite->currentAnimationName == "boot") {
    sprite->playAnimation("talking", true);

    textboxText->active = true;

    enterTimer.go();
    speakDelay.go(enterTimer.duration);
  }

  if (!textboxText->active) {
    return;
  }

  float offset = 0;

  if (!enterTimer.done() && enterTimer.running) {
    float pc = enterTimer.pc();
    pc *= pc;

    offset = 100.0 * (1.0 - pc);
  }

  float tbx = textboxX;
  float tby = textboxY + offset;

  textboxText->x = tbx + textboxTextOffsetX;
  textboxText->y = tby + textboxTextOffsetY;

  // Send render commands for textboxSprite
  for (int y = 0; y < textboxHeight; y++) {
    for (int x = 0; x < textboxWidth; x++) {
      int f = 4;

      if (x == 0 && y == 0) {
        f = 0;
      } else if (x == textboxWidth - 1 && y == textboxHeight - 1) {
        f = 8;
      } else if (x == textboxWidth - 1 && y == 0) {
        f = 2;
      } else if (x == 0 && y == textboxHeight - 1) {
        f = 6;
      } else if (x == 0) {
        f = 3;
      } else if (x == textboxWidth - 1) {
        f = 5;
      } else if (y == 0) {
        f = 1;
      } else if (y == textboxHeight - 1) {
        f = 7;
      }

      textboxSprite->frame = f;
      SDL_Rect src = textboxSprite->getSRC();

      Rect dst = {
        (float) (tbx + x * textboxSprite->frameWidth),
        (float) (tby + y * textboxSprite->frameWidth),
        (float) textboxSprite->frameWidth,
        (float) textboxSprite->frameWidth,
      };

      RenderJob j;
      j.depth = DEPTH_UI + DEPTH_BELOW;
      j.tex = textboxSprite->texture;
      j.src = src;
      j.dst = scene->camera->toView(dst, true);

      scene->renderer->queue.push(j);
    }
  }
}

void Boss::wakeUp() {
  awake = true;
  sprite->playAnimation("boot", false);
}
