#pragma once

#include <e/renderer.hpp>
#include <e/camera.hpp>

struct Scene {
  Renderer* renderer;
  Camera* camera;
};
