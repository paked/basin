#include <fake_panel.hpp>

FakePanel::FakePanel(float x, float y) {
  sprite = new Sprite("computer_room_wall_right.png", x, y);

}

void FakePanel::start() {
  reg(sprite);
}
