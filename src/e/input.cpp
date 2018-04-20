#include <e/input.hpp>

std::vector<Input*> Input::inputs;

void Input::handle(SDL_KeyboardEvent event) {
  for (auto input : inputs) {
    if (event.keysym.scancode == input->key) {
      input->thisFrameDown = event.state == SDL_PRESSED;
    }
  }
}

void Input::push() {
  for (auto input : inputs) {
    input->lastFrameDown = input->thisFrameDown;
  }
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
