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

  maxBufferHeight = (118) * scene->camera->zoom;

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
      .x = (int) (20 + screenRect.x),
      .y = (int) (124 * scene->camera->zoom + screenRect.y),
      .w = inputRect.w,
      .h = inputRect.h
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

  TTF_Font* font = Resources::get(25);

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

  TTF_Font* font = Resources::get(25);

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

  if (cmd == "look" && !wearingGoggles) {
    if (opts.empty()) {
      send(in, "You look around and see a trapdoor, a computer, and a pretty damn big cockroach.");

      return;
    } else if (opts == "cockroach") {
      send(in, "It's a pretty damn big cockroach. If you weren't some masochistic adventurer you'd definitely be jumping up on a table right about now. It seems to be alive, but isn't moving at the moment.");

      return;
    } else if (opts == "trapdoor" && !hasOpenedTrapdoor) {
      send(in, "It's a pretty rusty old trapdoor, it doesn't look like it's been used in at least a year. Maybe you can pull it open?");

      return;
    } else if (opts == "trapdoor" && hasOpenedTrapdoor) {
      send(in, "There's a really weird looking pair goggles of sitting inside the hole the trapdoor was concealing. They look a lot less worn than the trapdoor, come to think of it.");

      return;
    } else if (opts == "computer") {
      send(in, "There's some just-started-my-adventure looking peson using the computer right now, it's probably you. Let's steer clear of inception and stay away from him.");

      return;
    } else if (opts == "wall" || opts == "walls") {
      send(in, "They're pretty standard looking walls. Probably build a few hundred years ago by some monks named Mike. Unless you manage to find some x-ray goggles it's probably pretty pointless looking here.");

      return;
    } else if (opts == "goggles" || opts == "weird looking goggles") {
      send(in, "On a closer inspection these goggles look more like a VR headset than some cosplayers Steampunk outfit. Maybe if you grab and put them on they'll do something?");

      return;
    }

    send(in, "That thing doesn't seem to exist, to find things which do exist type \"look\"");
    return;
  }

  if (cmd == "look" && wearingGoggles) {
    if (opts.empty()) {
      send(in, "You look around the world and see, well, a lot of things. These might just be x-ray goggles! You can see the trapdoor you just opened, the circuits of the computer, the innards of the cockroach (and the weird cylindrical things which is inside it's body? maybe that's why it's not moving...) and the outlines of the walls to the room. You can see the hidden passage which you entered through, the meters of stone separating you from sunlight and... another hidden passage? it looks like there might be a fake panel!");

      return; 
    } else if (opts == "panel") {
      send(in, "A closer examination reveals that there's a small slot next to the window invisible to the naked eye.");

      return;
    }

    send(in, "That thing doesn't seem to exist -- even with x-ray vision, to find things which do exist type \"look\"");
  }

  if (cmd == "grab") {
    if (opts.empty()) {
      send(in, "You can't grab nothing, silly. To find something to try grab type \"look\"");

      return;
    } else if (opts == "cockroach") {
      send(in, "You may be a masochistic adventurer, but there's no way you're going to go anywhere near that cockroach without a good reason to. Things the damn biggest you've ever seen.");

      return;
    } else if (opts == "trapdoor" && !hasOpenedTrapdoor) {
      send(in, "It's pretty heavy, but you manage to pull it open. You think you probably should have had to find some key to open the lock with, but whatever. There's something inside it: a weird looking pair of goggles.");

      hasOpenedTrapdoor = true;

      return;
    } else if (opts == "trapdoor" && hasOpenedTrapdoor) {
      send(in, "The trapdoor has already being opened. I'm not sure what opening it again would look like.");

      return;
    } else if (opts == "goggles" || opts == "weird looking goggles") {
      send(in, "You grab the suprisingly pristine goggles out of the dusty compartment, and place them over your face. Lights start flashing and fans start whirring, you get the feeling the world might look a bit different now.");

      wearingGoggles = true;

      return;
    } else if (opts == "computer") {
      send(in, "What did I tell you about not intefering with the guy who is probably you using the computer?");

      return;
    }

    send(in, "That things doesn't seem to exist, to find things which do exist type \"look\"");

    return;
  }
  
  send(in, "Invalid command. Type \"help\" for some tips");
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
