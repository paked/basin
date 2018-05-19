#include <computer.hpp>

#include <e/resources.hpp>
#include <e/core.hpp>

#include <config.hpp>

void Computer::start() {
  localDepth = DEPTH_UI;

  screenSprite = new Sprite("computer_gui.png");
  screenSprite->hud = true;

  screenSprite->x = scene->camera->getWidth()/2 - screenSprite->width/2;
  screenSprite->y = scene->camera->getHeight()/2 - screenSprite->height/2;
  screenSprite->hud = true;

  screenRect = scene->camera->toView({ 32, 32, 128, 128 }, true);

  maxBufferHeight = (120) * scene->camera->zoom;

  send("It's damp, the floor is cold, and it smells of dead animal. Or something like that. Welcome to the innards of Echo Mountain. Around you is a small hole in the ground, a key, a computer, and a hammer.\n\n[If you don't know how to get started, type \"help\"]");

  genBuffer();
  reg(screenSprite);
}

void Computer::tick(float dt) {
  if (enter.justDown() && input != "") {
    eval(input);

    Input::stopTextInput();
    Input::startTextInput();
    // bufferOffset = 0;
  }

  if (up.justDown()) {
    bufferOffset -= 10;
  }

  if (down.justDown()) {
    bufferOffset += 10;
  }

  lastInput = input;
  input = Input::text;

  if (input != lastInput) {
    genInput();
  }

  // Rendering
  Entity::tick(dt);

  // Send main screen text
  {
    // If we need to scroll
    if (bufferRect.h > maxBufferHeight) {
      bufferViewport.h = maxBufferHeight;

      if (bufferOffset < 0) {
        bufferOffset = 0;

        printf("%d\n", bufferOffset);
      } else if (bufferOffset > bufferRect.h - maxBufferHeight) {
        bufferOffset = bufferRect.h - maxBufferHeight;
      }

      bufferViewport.y = bufferOffset;

      printf("%d\n", bufferViewport.y);
    }

    SDL_Rect pos = {
      .x = 18 + screenRect.x,
      .y = 25 + screenRect.y,
      .w = bufferViewport.w,
      .h = bufferViewport.h
    };

    RenderJob j;
    j.src = bufferViewport;
    j.dst = pos;
    j.tex = bufferTexture;
    j.depth = getDepth() + DEPTH_ABOVE;

    scene->renderer->queue.push(j);
  }
  
  // Send input line
  {
    SDL_Rect pos = {
      .x = 20 + screenRect.x,
      .y = 124 * scene->camera->zoom + screenRect.y,
      .w = (float) inputRect.w,
      .h = (float) inputRect.h
    };

    RenderJob j;
    j.src = inputRect;
    j.dst = pos;
    j.tex = inputTexture;
    j.depth = getDepth() + DEPTH_ABOVE;

    scene->renderer->queue.push(j);
  }
}

void Computer::activate() {
  active = true;

  Input::startTextInput();
}

void Computer::deactivate() {
  active = false;

  Input::stopTextInput();
}

void Computer::genBuffer() {
  SDL_DestroyTexture(bufferTexture);

  TTF_Font* font = Resources::get(35);

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, getBuffer().c_str(), textColor, 128 * scene->camera->zoom);

  bufferTexture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  bufferRect.x = bufferRect.y = 0;
  SDL_QueryTexture(bufferTexture, NULL, NULL, &bufferRect.w, &bufferRect.h);

  bufferViewport = bufferRect;
  bufferOffset = bufferRect.h - maxBufferHeight;

  SDL_FreeSurface(surface);
}

void Computer::genInput() {
  SDL_DestroyTexture(inputTexture);

  TTF_Font* font = Resources::get(35);

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, input.c_str(), textColor, 128 * scene->camera->zoom);

  inputTexture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  inputRect.x = inputRect.y = 0;
  SDL_QueryTexture(inputTexture, NULL, NULL, &inputRect.w, &inputRect.h);

  SDL_FreeSurface(surface);
}

void Computer::eval(std::string cmd) {
  if (cmd == "help") {
    send(cmd, "A soft voice from the distance whispers:\n\n- Type \"help\" to see this menu");
  } else {
    send(cmd, "Invalid command. Type \"help\" for some tips");
  }
}

void Computer::send(std::string cmd, std::string response) {
  if (response.size() > 0) {
    lines.push_back("> " + cmd);
    lines.push_back("$ " + response);
  } else {
    lines.push_back("$ " + cmd);
  }

  while (lines.size() > maxLines) {
    lines.erase(lines.begin());
  }

  genBuffer();
}

std::string Computer::getBuffer() {
  std::string res;

  for (int i = 0; i < lines.size(); i++) {
    res += lines[i];

    if (i != lines.size() - 1) {
      res += "\n";
    }
  }

  return res;
}
