#include "engine/renderer.hpp"
#include "engine/drawable.hpp"

void Renderer::add(const Drawable& d) {
  drawables.push_back(&d);
}

void Renderer::drawAll() const {
  for (const Drawable* d : drawables) {
    d->draw();
  }
}