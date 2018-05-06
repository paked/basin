#include <e/renderer.hpp>

#include <stdio.h>

#include <e/core.hpp>

void Renderer::render(SDL_Renderer* r) {
  while(!queue.empty()) {
    RenderJob next = queue.top();

    SDL_RenderCopyEx(r, next.tex, &next.src, &next.dst, next.angle, NULL, next.flip);

    queue.pop();
  }
}
