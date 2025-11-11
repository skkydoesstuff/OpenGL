#pragma once

#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"
#include "engine/scene.hpp"

class Application {
private:
  Window window;
  Shader shader;
  Renderer renderer;
  Camera camera;
  Scene scene;

public:
  Application();

  void start();
  void update();
  void setupGeometry();
  void render();

  void run();
};