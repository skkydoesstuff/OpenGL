#pragma once

#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/renderer.hpp"

class Application {
private:
  Window window;
  Shader shader;
  Renderer renderer;

public:

  Application();

  void run();
};