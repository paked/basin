#include <computer.hpp>

#include <e/resources.hpp>
#include <e/core.hpp>

#include <config.hpp>

void Computer::start() {
  localDepth = DEPTH_UI;

  screenSprite = new Sprite("computer_gui.png");
  screenSprite->hud = true;
  screenSprite->localDepth = DEPTH_BELOW;

  screenSprite->x = scene->camera->getWidth()/2 - screenSprite->width/2;
  screenSprite->y = scene->camera->getHeight()/2 - screenSprite->height/2;
  screenSprite->hud = true;

  screenRect = scene->camera->toView({ 32, 32, 128, 128 }, true);

  maxBufferHeight = (120) * scene->camera->zoom;

  send("The rocky floor is cold, it smells of dead animal, and the sound of water is dripping in the distance. You're in a cavern deep inside Echo Mountain delivering a battery to the \"tech spirit\" so it can stay powered on another year. You look around the room and see a trapdoor, a computer, and a pretty damn big cockroach.\n\n[If you don't know how to get started, type \"help\"]");

  genBuffer();

  reg(screenSprite);
}

void Computer::tick(float dt) {
  if (enter.justDown() && input != "") {
    eval(input);

    Input::stopTextInput();
    Input::startTextInput();
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
      } else if (bufferOffset > bufferRect.h - maxBufferHeight) {
        bufferOffset = bufferRect.h - maxBufferHeight;
      }

      bufferViewport.y = bufferOffset;
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

  TTF_Font* font = Resources::get(15);

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, getBuffer().c_str(), textColor, 128 * scene->camera->zoom);

  bufferTexture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  bufferRect.x = bufferRect.y = 0;
  SDL_QueryTexture(bufferTexture, NULL, NULL, &bufferRect.w, &bufferRect.h);

  bufferRect.h *= 0.9;
  bufferViewport = bufferRect;
  bufferOffset = bufferRect.h - maxBufferHeight;

  SDL_FreeSurface(surface);
}

void Computer::genInput() {
  SDL_DestroyTexture(inputTexture);

  TTF_Font* font = Resources::get(15);

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, input.c_str(), textColor, 128 * scene->camera->zoom);

  inputTexture = SDL_CreateTextureFromSurface(Core::renderer, surface);

  inputRect.x = inputRect.y = 0;
  SDL_QueryTexture(inputTexture, NULL, NULL, &inputRect.w, &inputRect.h);

  SDL_FreeSurface(surface);
}

void Computer::eval(std::string in) {
  std::string cmd;
  std::string opts;

  for (int i = 0; i < in.size(); i++) {
    char c = in[i];
    if (c == ' ') {
      opts = in.substr(i + 1);

      break;
    }

    cmd += c;
  }

  if (cmd == "help") {
    send(in, "This is a help screen. Congratulations, you can type. These are other things you can type: \n\n- \"help\" to see this menu\n- \"look <something>\" to at something (eg. \"look cockroach\", \"look trapdoor\", \"look computer\"). You can also look around the room with just \"look\"\n- \"grab\" to pick up or pull on an item (eg \"grab trapdoor\", \"grab cockroach\")");

    return;
  }

  if (cmd == "look") {
    if (opts.empty()) {
      send(in, "You look around and see a trapdoor, a computer, and a pretty damn big cockroach.");

      return;
    } else if (opts == "cockroach") {
      send(in, "It's a pretty damn big cockroach. If you weren't some masochistic adventurer you'd definitely be jumping up on a table right about now. It seems to be alive, but isn't moving at the moment.");

      return;
    } else if (opts == "trapdoor") {
      send(in, "It's a pretty rusty old trapdoor, it doesn't look like it's been used in at least a year. Maybe you can pull it open?");

      return;
    } else if (opts == "computer") {
      send(in, "There's some just-started-my-adventure looking peson using the computer right now, it's probably you. Let's steer clear of inception and stay away from him.");

      return;
    } else if (opts == "wall" || opts == "walls") {
      send(in, "They're pretty standard looking walls. Probably build a few hundred years ago by some monks named Mike. Unless you manage to find some x-ray goggles it's probably pretty pointless looking here.");

      return;
    }

    send(in, "That thing doesn't seem to exist, to find things which do exist type \"look\"");
    return;
  }
  
  send(cmd, "Invalid command. Type \"help\" for some tips");
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
