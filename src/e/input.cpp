#include <e/input.hpp>

#include <config.hpp>

std::vector<Input*> Input::inputs;

int Input::mouseX = 0;
int Input::mouseY = 0;

bool Input::mouseLastFrameDown = false;
bool Input::mouseThisFrameDown = false;

void Input::handle(SDL_KeyboardEvent event) {
  for (auto input : inputs) {
    if (event.keysym.scancode == input->key) {
      input->thisFrameDown = event.state == SDL_PRESSED;
    }
  }
}

void Input::handle(SDL_MouseMotionEvent event) {
  mouseX = event.x / GAME_SCALE;
  mouseY = event.y / GAME_SCALE;
}

void Input::handle(SDL_MouseButtonEvent event) {
  if (event.button != SDL_BUTTON_LEFT) {
    // TODO: properly handle mouse buttons

    return;
  }

  mouseThisFrameDown = event.state == SDL_PRESSED;
}

void Input::push() {
  mouseLastFrameDown = mouseThisFrameDown;

  for (auto input : inputs) {
    input->lastFrameDown = input->thisFrameDown;
  }
}

bool Input::mouseJustDown() {
  return !mouseLastFrameDown && mouseThisFrameDown;
}

bool Input::mouseJustUp() {
  return mouseLastFrameDown && !mouseThisFrameDown;
}

bool Input::mouseDown() {
  return mouseThisFrameDown;
}

bool Input::mouseUp() {
  return !mouseThisFrameDown;
}


Input::Input(SDL_Scancode k) : key(k) {
  inputs.push_back(this);
};

bool Input::down() {
  return thisFrameDown;
}

bool Input::up() {
  return !thisFrameDown;
}

bool Input::justDown() {
  return !lastFrameDown && thisFrameDown;
}

bool Input::justUp() {
  return lastFrameDown && !thisFrameDown;
}
